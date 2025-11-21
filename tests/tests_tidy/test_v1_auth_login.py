import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME


@pytest.mark.pgsql(DB_NAME, files=['users.sql'])
async def test_login(service_client: Client):
    response = await service_client.post(
        '/v1/auth/login',
        json={
            'email': 'slayy@gmail.com',
            'password': '777cantfindme',
        }
    )

    assert response.status == 200
    assert response.json() == {}

    response = await service_client.post(
        '/v1/auth/login',
        json={
            'email': 'slayer_clone@gmail.com',
            'password': '777cantfindme',
        }
    )

    assert response.status == 401
    assert response.json()['message'] == 'EMAIL_NOT_FOUND'
