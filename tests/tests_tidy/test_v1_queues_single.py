import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql', 'queues.sql'])
@pytest.mark.parametrize(
    "queue_id, response_status, error_response_json, should_fail",
    [
        pytest.param(
            '1e6fd243-6239-4d76-8f0d-2bcdbc194f18',
            200,
            None,
            False,
            id='ok by user permission'
        ),
        pytest.param(
            'c7838595-1516-4d86-a22a-6bac8cc46961',
            200,
            None,
            False,
            id='ok by role permission'
        ),
        pytest.param(
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            404,
            {'code': '404', 'message': 'QUEUE_NOT_FOUND'},
            True,
            id='no access'
        ),
        pytest.param(
            'i_am_invalid',
            400,
            {'code': '400', 'message': 'INVALID_QUEUE_ID'},
            True,
            id='broken queue id'
        ),
    ],
)
async def test_get_single_queue(
    service_client: Client,
    queue_id,
    response_status,
    error_response_json,
    should_fail,
):
    response = await service_client.get(
        f'/v1/queues/{queue_id}',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'}
    )

    assert response.status == response_status
    if should_fail:
        assert response.json() == error_response_json
