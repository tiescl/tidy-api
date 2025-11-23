import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql'])
@pytest.mark.parametrize(
    "url, response_status, response_user_cnt, check_potato_electro",
    [
        pytest.param(
            '/admin/v1/users?limit=2',
            200,
            2,
            False,
            id='simple limit'
        ),
        pytest.param(
            '/admin/v1/users?limit=4&search=potato',
            200,
            1,
            False,
            id='limit with search'
        ),
        pytest.param(
            '/admin/v1/users?limit=100&search=gmail',
            200,
            4,
            False,
            id='large limit'
        ),
        pytest.param(
            '/admin/v1/users?page=2&limit=2',
            200,
            2,
            True,
            id='limit with page'
        ),
    ],
)
async def test_admin_get_users(
    service_client: Client,
    url: str,
    response_status: int,
    response_user_cnt: int,
    check_potato_electro: bool,
):
    response = await service_client.get(
        url,
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'},
    )

    assert response.status == response_status
    assert len(response.json()['users']) == response_user_cnt

    if check_potato_electro:
        users = response.json()['users']
        for user in users:
            assert user['username'] in [
                'potato_mushroom',
                'harmonic_electrode'
            ]


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql'])
async def test_admin_delete_user(service_client: Client):
    response = await service_client.delete(
        '/admin/v1/users',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'},
        json={'user_id': '605223cd-826a-46a7-9398-b21f1dd4fd45'}
    )

    assert response.status == 200

    response = await service_client.delete(
        '/admin/v1/users',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'},
        json={'user_id': '605223cd-826a-46a7-9398-b21f1dd4abcd'}
    )

    assert response.status == 404
    assert response.json() == {'code': '404', 'message': 'USER_NOT_FOUND'}
