import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME
from tests.tests_tidy.utils import remove_ids_and_timestamps


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql', 'queues.sql'])
@pytest.mark.parametrize(
    "queue_id, request_body, response_status, response_json, should_fail, error_response_json",
    [
        pytest.param(
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            {
                'title': 'Refactor code in /v1/auth',
                'description': 'After the new intern guy wrote some new '
                               'features there the code became super hard '
                               'to read. Let\'s get rid of that BS ASAP',
                'component': 'tidy_development_team'
            },
            200,
            'ok_owner_response.json',
            False,
            None,
            id='ok owner'
        ),
        pytest.param(
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            {
                'title': 'Refactor code in /v1/auth',
                'description': 'After the new intern guy wrote some new '
                               'features there the code became super hard '
                               'to read. Let\'s get rid of that BS ASAP',
                'component': 'tidy_development_team',
                'assignee_id': '774cc050-2542-4c05-9027-f4919112fc81',
                'type': 'task',
                'status': 'planned',
                'priority': 'critical',
                'story_points': 12,
            },
            200,
            'ok_all_columns_response.json',
            False,
            None,
            id='ok all columns'
        ),
        pytest.param(
            '8cb68c39-1cd0-4a57-960d-c13f3b5c5e4a',
            {
                'title': 'Clicking on profile icon leads to 404 screen',
                'description': 'Hi! Can you please look into this, '
                               'we have received over 15 complaints '
                               'over the last 20 minutes about this',
                'component': 'tidy_development_team'
            },
            200,
            'ok_user_perm_response.json',
            False,
            None,
            id='ok user perm'
        ),
        pytest.param(
            '57996a05-fb8c-454d-910e-8b53d313741e',
            {
                'title': 'Изменения в архитектуру tidy',
                'description': 'Меняем флоу отправки нотификаций. '
                               'Нужно написать крон таску, которая '
                               'будет читать сообщения из RabbitMQ',
                'component': 'tidy_development_team'
            },
            200,
            'ok_role_perm_response.json',
            False,
            None,
            id='ok role perm'
        ),
        pytest.param(
            '5bf11df6-3c74-4962-a272-aed7c9e35f22',
            {
                'title': 'Refactor code for the landing page',
                'description': 'After the new intern guy wrote some new '
                               'features there the code became super hard '
                               'to read. Let\'s get rid of that BS ASAP',
                'component': 'tidy_development_team'
            },
            403,
            None,
            True,
            {'code': '403', 'message': 'FORBIDDEN'},
            id='no create perm'
        ),
        pytest.param(
            '69f11df6-3c74-4962-a272-aed7c9e35f22',
            {
                'title': 'Refactor code for the landing page',
                'description': 'After the new intern guy wrote some new '
                               'features there the code became super hard '
                               'to read. Let\'s get rid of that BS ASAP',
                'component': 'tidy_development_team'
            },
            404,
            None,
            True,
            {'code': '404', 'message': 'QUEUE_NOT_FOUND'},
            id='non-existent queue id'
        ),
        pytest.param(
            '5bf11df6-3c74-4962-a272-aed7c9e35f22',
            {
                'title': 'Refactor code for the landing page',
                'description': 'After the new intern guy wrote some new '
                               'features there the code became super hard '
                               'to read. Let\'s get rid of that BS ASAP',
                'component': 'tidy_development_team',
                'assignee_id': '5c4b6cc6-10f9-4663-b473-3ffd8cc225f5'
            },
            404,
            None,
            True,
            {'code': '404', 'message': 'USER_NOT_FOUND'},
            id='removed assignee'
        ),
    ],
)
async def test_create_issue(
    service_client: Client,
    load_json,
    queue_id,
    request_body,
    response_status,
    response_json,
    should_fail,
    error_response_json,
):
    response = await service_client.post(
        f'/v1/queues/{queue_id}/issues',
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
