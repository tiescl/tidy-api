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

    # duplicate email
    response = await service_client.post(
        '/v1/auth/register',
        json={
            'username': 'simple77',
            'email': 'simple@gmail.com',
            'password': '777cantfindme',
        }
    )

    assert response.status == 400
    assert response.json()['message'] == "EMAIL_ALREADY_EXISTS"

    # duplicate username
    response = await service_client.post(
        '/v1/auth/register',
        json={
            'username': 'simple777',
            'email': 'simple2@gmail.com',
            'password': '777cantfindme',
        }
    )

    assert response.status == 400
    assert response.json()['message'] == "USERNAME_ALREADY_EXISTS"
