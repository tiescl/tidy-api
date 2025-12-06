import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql', 'queues.sql', 'issues.sql'])
@pytest.mark.parametrize(
    "queue_id, response_status, response_json, should_fail, error_response_json",
    [
        pytest.param(
            '27c35e0f-af78-4994-9875-35e875790d36',
            404,
            None,
            True,
            {'code': '404', 'message': 'QUEUE_NOT_FOUND'},
            id='queue does not exist',
        ),
        pytest.param(
            '1e6fd243-6239-4d76-8f0d-2bcdbc194f18',
            200,
            'queue_without_issues_response.json',
            False,
            None,
            id='queue without issues'
        ),
        pytest.param(
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            200,
            'queue_with_issues_response.json',
            False,
            None,
            id='queue with issues'
        ),
    ],
)
async def test_get_queue_issues(
    service_client: Client,
    load_json,
    queue_id,
    response_status,
    response_json,
    should_fail,
    error_response_json,
):
    response = await service_client.get(
        f'/v1/queues/{queue_id}/issues',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'}
    )

    assert response.status == response_status
    if should_fail:
        assert response.json() == error_response_json
    else:
        assert response.json() == load_json(response_json)
