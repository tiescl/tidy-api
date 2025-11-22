import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql'])
async def test_get_current_user(service_client: Client):
    response = await service_client.get(
        f'/v1/users/me',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'}
    )

    assert response.status == 200
    assert response.json() == {
        'email': 'demon_slayer',
        'id': '774cc050-2542-4c05-9027-f4919112fc81',
        'role': 'admin',
        'username': 'qwer@gmail.com',
    }

    response = await service_client.get(
        f'/v1/users/me',
        headers={'Cookie': 'session_token=f47116c18a9345a0a2b5ea97fbc4e8f0'}
    )

    assert response.status == 404
    assert response.json() == {'code': '404', 'message': 'USER_NOT_FOUND'}
