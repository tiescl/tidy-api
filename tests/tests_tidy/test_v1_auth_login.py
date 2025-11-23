import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME


@pytest.mark.pgsql(DB_NAME, files=['users.sql'])
@pytest.mark.parametrize(
    'request_body, response_status, response_json',
    [
        pytest.param(
            {
                'email': 'slayy@gmail.com',
                'password': '777cantfindme',
            },
            200,
            {},
            id='ok',
        ),
        pytest.param(
            {
                'email': 'slayer_clone@gmail.com',
                'password': '777cantfindme',
            },
            401,
            {'code': '401', 'message': 'EMAIL_NOT_FOUND'},
            id='email not found',
        ),
        pytest.param(
            {
                'email': 'slayy@gmail.com',
                'password': '333cantfindme',
            },
            401,
            {'code': '401', 'message': 'INVALID_PASSWORD'},
            id='wrong password',
        ),
        pytest.param(
            {
                'email': 'potato@gmail.com',
                'password': '777cantfindme',
            },
            401,
            {'code': '401', 'message': 'EMAIL_NOT_FOUND'},
            id='deleted user',
        ),
    ],
)
async def test_login(
    service_client: Client,
    request_body,
    response_status,
    response_json,
):
    response = await service_client.post(
        '/v1/auth/login',
        json=request_body,
    )

    assert response.status == response_status
    assert response.json() == response_json
