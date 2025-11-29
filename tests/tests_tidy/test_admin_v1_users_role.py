import typing
import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql'])
@pytest.mark.parametrize(
    'user_id, role, response_status, response_json',
    [
        pytest.param(
            '605223cd-826a-46a7-9398-b21f1dd4fd45',
            'developer',
            200,
            {},
            id='ok',
        ),
        pytest.param(
            'invalid-uuid',
            'developer',
            400,
            {'code': '400', 'message': 'INVALID_USER_ID'},
            id='invalid uuid',
        ),
        pytest.param(
            '695223cd-826a-46a7-9398-b21f1dd4fd45',
            'developer',
            404,
            {'code': '404', 'message': 'USER_NOT_FOUND'},
            id='user purged',
        ),
    ]
)
async def test_admin_update_user_role(
    service_client: Client,
    user_id: str,
    role: str,
    response_status: int,
    response_json: typing.Dict,
):
    response = await service_client.post(
        f'/admin/v1/users/{user_id}/role',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'},
        json={'role': role}
    )

    assert response.status == response_status
    assert response.json() == response_json
