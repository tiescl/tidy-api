import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME
from tests.tests_tidy.utils import remove_ids_and_timestamps


@pytest.mark.pgsql(DB_NAME, files=[
    'users.sql',
    'tokens.sql',
    'queues.sql',
    'issues.sql'
])
@pytest.mark.parametrize(
    (
        'queue_id',
        'issue_id',
        'request_body',
        'response_status',
        'response_json',
        'should_fail',
        'error_response_json',
    ),
    [
        pytest.param(
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            '264c7fc0-6020-48ae-b10c-d0d1479d9355',
            {
                'title': 'Not just some backend task, '
                'it is the first ever ticket in TIDYBACKEND'
            },
            200,
            'ok_owner_response.json',
            False,
            None,
            id='ok owner'
        ),
        pytest.param(
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            '264c7fc0-6020-48ae-b10c-d0d1479d9355',
            {
                'title': 'Test task in TIDYBACKEND',
                'description': 'Test description for TIDYBACKEND-1',
                'type': 'epic',
                'status': 'in_progress',
                'priority': 'critical',
                'component': 'tidy_development_team',
                'story_points': 10,
                'assignee_id': 'a9f70642-6e70-4c86-97ff-455d659bede0',
            },
            200,
            'ok_all_columns_response.json',
            False,
            None,
            id='ok all columns'
        ),
        pytest.param(
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            '264c7fc0-6020-48ae-b10c-d0d1479d9355',
            {},
            200,
            'ok_zero_columns_response.json',
            False,
            None,
            id='ok zero columns'
        ),
        pytest.param(
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            '264c7fc0-6020-48ae-b10c-d0d1479d9355',
            {
                'priority': 'damn_important',
            },
            500,
            None,
            True,
            {
                'code': '500',
                'details': "Error at path 'priority': Invalid enum value "
                "(damn_important) for type ::defs::issues::IssuePriority",
                'message': 'Internal Server Error',
            },
            id='invalid payload'
        ),
        pytest.param(
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            '264c7fc0-6020-48ae-b10c-d0d1479d9355',
            {
                'assignee_id': '5c4b6cc6-10f9-4663-b473-3ffd8cc225f5',
            },
            404,
            None,
            True,
            {'code': '404', 'message': 'USER_NOT_FOUND'},
            id='assignee is removed'
        ),
        pytest.param(
            '5bf11df6-3c74-4962-a272-aed7c9e35f22',
            '5fe28b12-824e-4c0a-aee4-f98e9b9aacbc',
            {
                'title': 'shiny new title',
            },
            200,
            'ok_edit_any_perm_response.json',
            False,
            None,
            id='ok edit_any perm'
        ),
        pytest.param(
            '57996a05-fb8c-454d-910e-8b53d313741e',
            '4dbef3e1-011a-4536-9473-42b414e1bf79',
            {
                'title': 'shiny new title',
            },
            200,
            'ok_edit_own_perm_response.json',
            False,
            None,
            id='ok edit_own perm'
        ),
        pytest.param(
            '57996a05-fb8c-454d-910e-8b53d313741e',
            'f6dd657a-e5d1-4851-8a18-14e6e2d2976a',
            {
                'title': 'shiny new title',
            },
            403,
            None,
            True,
            {'code': '403', 'message': 'FORBIDDEN'},
            id='no perms'
        ),
        pytest.param(
            '5bf11df6-3c74-4962-a272-aed7c9e35f22',
            'f6dd657a-e5d1-4851-8a18-14e6e2d2976a',
            {
                'title': 'shiny new title',
            },
            404,
            None,
            True,
            {'code': '404', 'message': 'ISSUE_NOT_FOUND'},
            id='issue belongs in another queue'
        ),
        pytest.param(
            '57996a05-fb8c-454d-910e-8b53d313741e',
            'd239c9f2-7cf8-408c-aed1-69bda4f8c256',
            {
                'title': 'shiny new title',
            },
            404,
            None,
            True,
            {'code': '404', 'message': 'ISSUE_NOT_FOUND'},
            id='issue removed'
        ),
        pytest.param(
            '6f39866a-e5a5-4e91-ad00-e31163cea1c1',
            'd239c9f2-7cf8-408c-aed1-69bda4f8c256',
            {
                'title': 'shiny new title',
            },
            404,
            None,
            True,
            {'code': '404', 'message': 'QUEUE_NOT_FOUND'},
            id='queue removed'
        ),
    ],
)
async def test_edit_issue(
    service_client: Client,
    load_json,
    queue_id,
    issue_id,
    request_body,
    response_status,
    response_json,
    should_fail,
    error_response_json,
):
    response = await service_client.put(
        f'/v1/queues/{queue_id}/issues/{issue_id}',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'},
        json=request_body,
    )

    assert response.status == response_status
    response = response.json()
    remove_ids_and_timestamps(response)

    if should_fail:
        assert response == error_response_json
    else:
        print(response)
        assert response == load_json(response_json)
