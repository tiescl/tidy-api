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
        'email': 'qwer@gmail.com',
        'id': '774cc050-2542-4c05-9027-f4919112fc81',
        'role': 'admin',
        'username': 'demon_slayer',
    }

    response = await service_client.get(
        f'/v1/users/me',
        headers={'Cookie': 'session_token=f47116c18a9345a0a2b5ea97fbc4e8f0'}
    )

    assert response.status == 404
    assert response.json() == {'code': '404', 'message': 'USER_NOT_FOUND'}


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql'])
@pytest.mark.parametrize(
    "request_body, response_status, response_json",
    [
        pytest.param(
            {
                'full_name': 'Derek Wilson',
                'email': 'iuseyahoonow@yahoo.com',
                'username': 'not_a_demon_slayer',
            },
            200,
            {
                'id': '774cc050-2542-4c05-9027-f4919112fc81',
                'full_name': 'Derek Wilson',
                'email': 'iuseyahoonow@yahoo.com',
                'username': 'not_a_demon_slayer',
                'role': 'admin',
            },
            id='ok all fields'
        ),
        pytest.param(
            {
                'email': 'iuseyandexnow@yandex.com',
                'username': 'still_a_slayer',
            },
            200,
            {
                'id': '774cc050-2542-4c05-9027-f4919112fc81',
                'email': 'iuseyandexnow@yandex.com',
                'username': 'still_a_slayer',
                'role': 'admin',
            },
            id='ok without full_name'
        ),
        pytest.param(
            {
                'username': 'nope_not_a_slayer',
            },
            200,
            {
                'id': '774cc050-2542-4c05-9027-f4919112fc81',
                'email': 'qwer@gmail.com',
                'username': 'nope_not_a_slayer',
                'role': 'admin',
            },
            id='ok just username'
        ),
        pytest.param(
            {
                'full_name': 'John Wilson'
            },
            200,
            {
                'id': '774cc050-2542-4c05-9027-f4919112fc81',
                'full_name': 'John Wilson',
                'email': 'qwer@gmail.com',
                'username': 'demon_slayer',
                'role': 'admin',
            },
            id='ok just full name'
        ),
        pytest.param(
            {},
            200,
            {
                'id': '774cc050-2542-4c05-9027-f4919112fc81',
                'email': 'qwer@gmail.com',
                'username': 'demon_slayer',
                'role': 'admin',
            },
            id='ok empty req body'
        ),
        pytest.param(
            {
                'email': 'electro@gmail.com'
            },
            400,
            {
                'code': '400',
                'message': 'EMAIL_ALREADY_EXISTS'
            },
            id='email taken'
        ),
        pytest.param(
            {
                'username': 'harmonic_electrode'
            },
            400,
            {
                'code': '400',
                'message': 'USERNAME_ALREADY_EXISTS'
            },
            id='username taken'
        )
    ]
)
async def test_update_current_user(
    service_client: Client,
    request_body,
    response_status,
    response_json,
):
    response = await service_client.put(
        f'/v1/users/me',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'},
        json=request_body,
    )

    assert response.status == response_status
    assert response.json() == response_json


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql'])
async def test_get_curr_user_token_expired(service_client: Client):
    response = await service_client.get(
        '/v1/users/me',
        headers={'Cookie': 'session_token=f57116c18a9345a0a2b5ea97fbc4e8f0'}
    )

    assert response.status == 401
    assert response.json() == {'code': '401', 'message': 'INVALID_TOKEN'}


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql'])
async def test_delete_user(service_client: Client):
    response = await service_client.delete(
        '/v1/users/me',
        headers={'Cookie': 'session_token=f67116c18a9345a0a2b5ea97fbc4e8f0'}
    )

    assert response.status == 200
