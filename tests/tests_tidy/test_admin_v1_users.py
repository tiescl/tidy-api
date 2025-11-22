import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql'])
async def test_admin_get_users(service_client: Client):
    response = await service_client.get(
        '/admin/v1/users?limit=2',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'},
    )

    assert response.status == 200
    assert len(response.json()['users']) == 2

    response = await service_client.get(
        '/admin/v1/users?limit=4&search=potato',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'},
    )

    assert response.status == 200
    assert len(response.json()['users']) == 1

    response = await service_client.get(
        '/admin/v1/users?limit=100&search=gmail',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'},
    )

    assert response.status == 200
    assert len(response.json()['users']) == 4

    response = await service_client.get(
        '/admin/v1/users?page=2&limit=2',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'},
    )

    assert response.status == 200

    users = response.json()['users']
    assert len(users) == 2
    for user in users:
        assert user['username'] in ('potato_mushroom', 'harmonic_electrode')
