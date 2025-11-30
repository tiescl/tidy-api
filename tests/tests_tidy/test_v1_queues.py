import typing
import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql', 'queues.sql'])
@pytest.mark.parametrize(
    "session_token, request_body, response_status, expected_response_json, should_fail, error_response_json",
    [
        pytest.param(
            'f37116c18a9345a0a2b5ea97fbc4e8f0',
            {
                'key': 'TIDYFRONT',
                'name': 'Queue for tracking front end related tickets for tidy',
                'owner_id': '774cc050-2542-4c05-9027-f4919112fc81',
            },
            200,
            'ok_response.json',
            False,
            None,
            id='ok'
        ),
        pytest.param(
            'f37116c18a9345a0a2b5ea97fbc4e8f0',
            {
                'key': 'TIDYFRONT',
                'name': 'Queue for tracking front end related tickets for tidy',
                'owner_id': '775cc050-2542-4c05-9027-f4919112fc81',
            },
            400,
            None,
            True,
            {'code': '400', 'message': 'QUEUE_OWNER_MISMATCH'},
            id='owner_id mismatch'
        ),
        pytest.param(
            'f37116c18a9345a0a2b5ea97fbc4e8f0',
            {
                'key': 'TIDYBACK',
                'name': 'Queue for tracking back end related tickets for tidy',
                'owner_id': '774cc050-2542-4c05-9027-f4919112fc81',
            },
            400,
            None,
            True,
            {'code': '400', 'message': 'QUEUE_ALREADY_EXISTS'},
            id='key unique violation'
        ),
        pytest.param(
            'f47116c18a9345a0a2b5ea97fbc4e8f0',
            {
                'key': 'TIDYFRONT',
                'name': 'Queue for tracking front end related tickets for tidy',
                'owner_id': '5ac15ada-0726-4590-92b8-59e0ef096afa',
            },
            400,
            None,
            True,
            {'code': '400', 'message': 'USER_NOT_FOUND'},
            id='removed user'
        ),
        pytest.param(
            'invalid_session_token',
            {
                'key': 'TIDYFRONT',
                'name': 'Queue for tracking front end related tickets for tidy',
                'owner_id': '5ac15ada-0726-4590-92b8-59e0ef096afa',
            },
            401,
            None,
            True,
            {'code': '401', 'message': 'INVALID_TOKEN'},
            id='invalid session token'
        ),
        pytest.param(
            'f37116c18a9345a0a2b5ea97fbc4e8f0',
            {
                'key': 'tidyfront',
                'name': 'Queue for tracking front end related tickets for tidy',
                'owner_id': '774cc050-2542-4c05-9027-f4919112fc81',
            },
            500,
            None,
            True,
            {
                'code': '500',
                'details': "Error at path 'key': doesn't match regex",
                'message': 'Internal Server Error'
            },
            id='lowercase key'
        ),
        pytest.param(
            'f37116c18a9345a0a2b5ea97fbc4e8f0',
            {
                'key': 'TIDYFRONT',
                'name': 'que',
                'owner_id': '774cc050-2542-4c05-9027-f4919112fc81',
            },
            500,
            None,
            True,
            {
                'code': '500',
                'details': "Error at path 'name': Too short string, minimum length=4, given=3",
                'message': 'Internal Server Error'
            },
            id='short name'
        ),
    ],
)
async def test_create_queue(
    service_client: Client,
    load_json,
    session_token: str,
    request_body: typing.Dict,
    response_status: int,
    expected_response_json: str,
    should_fail: bool,
    error_response_json: typing.Dict,
):
    response = await service_client.post(
        f'/v1/queues',
        headers={'Cookie': f'session_token={session_token}'},
        json=request_body,
    )

    assert response.status == response_status
    if should_fail:
        assert response.json() == error_response_json
    else:
        response = response.json()
        expected_response: typing.Dict = load_json(expected_response_json)

        assert response['key'] == expected_response['key']
        assert response['name'] == expected_response['name']
        assert response['owner'] == expected_response['owner']


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql', 'queues.sql'])
@pytest.mark.parametrize(
    "session_id, queue_id, response_status, error_response_json, should_fail",
    [
        pytest.param(
            'f37116c18a9345a0a2b5ea97fbc4e8f0',
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            200,
            None,
            False,
            id='ok'
        ),
        pytest.param(
            'f47116c18a9345a0a2b5ea97fbc4e8f0',
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            400,
            {'code': '400', 'message': 'QUEUE_OWNER_MISMATCH'},
            True,
            id='owner mismatch'
        ),
        pytest.param(
            'f37116c18a9345a0a2b5ea97fbc4e8f0',
            '6d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            404,
            {'code': '404', 'message': 'QUEUE_NOT_FOUND'},
            True,
            id='not found'
        ),
    ],
)
async def test_delete_queue(
    service_client: Client,
    session_id: str,
    queue_id: str,
    response_status: int,
    error_response_json: typing.Dict,
    should_fail: bool
):
    response = await service_client.post(
        f'/v1/queues/{queue_id}/delete',
        headers={'Cookie': f'session_token={session_id}'},
    )

    assert response.status == response_status
    if should_fail:
        assert response.json() == error_response_json
    else:
        assert response.json() == {}
