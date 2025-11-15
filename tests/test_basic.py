from testsuite.daemons.service_client import Client


async def test_basic(service_client: Client):
    response = await service_client.post(
        '/v1/auth/register',
        json={
            'username': 'simple777',
            'email': 'simple@gmail.com',
            'password': '777cantfindme',
        }
    )

    assert response.status == 200
    assert 'id' in response.json()
