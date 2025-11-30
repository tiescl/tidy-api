import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME


@pytest.mark.pgsql(DB_NAME, files=[
    'users.sql',
    'tokens.sql',
    'queues.sql',
    'issues.sql',
])
@pytest.mark.parametrize(
    (
        'queue_id',
        'issue_id',
        'response_status',
        'response_json',
        'should_fail',
        'error_response_json',
    ),
    [
        pytest.param(
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            '264c7fc0-6020-48ae-b10c-d0d1479d9355',
            200,
            'ok_owner_response.json',
            False,
            None,
            id='ok owner'
        ),
        pytest.param(
            '1e6fd243-6239-4d76-8f0d-2bcdbc194f18',
            'fb18e7f4-2cfd-4cf8-bbf0-ab575f2a1e6c',
            200,
            'ok_user_perm_response.json',
            False,
            None,
            id='ok user perm'
        ),
        pytest.param(
            '1e6fd243-6239-4d76-8f0d-2bcdbc194f18',
            '264c7fc0-6020-48ae-b10c-d0d1479d9355',
            404,
            None,
            True,
            {'code': '404', 'message': 'ISSUE_NOT_FOUND'},
            id='issue belongs in another queue'
        ),
        pytest.param(
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            '076a4461-5faf-41c3-b061-de510fc2d409',
            404,
            None,
            True,
            {'code': '404', 'message': 'ISSUE_NOT_FOUND'},
            id='issue removed'
        ),
        pytest.param(
            '5bf11df6-3c74-4962-a272-aed7c9e35f22',
            '5fe28b12-824e-4c0a-aee4-f98e9b9aacbc',
            404,
            None,
            True,
            {'code': '404', 'message': 'QUEUE_NOT_FOUND'},
            id='queue removed'
        ),
        pytest.param(
            '57996a05-fb8c-454d-910e-8b53d313741e',
            '4dbef3e1-011a-4536-9473-42b414e1bf79',
            403,
            None,
            True,
            {'code': '403', 'message': 'FORBIDDEN'},
            id='no perms'
        ),
    ],
)
async def test_get_issue(
    service_client: Client,
    load_json,
    queue_id,
    issue_id,
    response_status,
    response_json,
    should_fail,
    error_response_json,
):
    response = await service_client.get(
        f'/v1/queues/{queue_id}/issues/{issue_id}',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'}
    )

    assert response.status == response_status
    if should_fail:
        assert response.json() == error_response_json
    else:
        assert response.json() == load_json(response_json)
