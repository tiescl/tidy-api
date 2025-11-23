import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME


@pytest.mark.pgsql(DB_NAME, files=['users.sql'])
@pytest.mark.parametrize(
    "request_body, response_status, error_response_json, should_fail",
    [
        pytest.param(
            {
                'username': 'simple777',
                'email': 'simple@gmail.com',
                'password': '777cantfindme',
            },
            200,
            None,
            False,
            id='ok'
        ),
        pytest.param(
            {
                'username': 'simple777',
                'email': 'slayy@gmail.com',
                'password': '777cantfindme',
            },
            400,
            {'code': '400', 'message': 'EMAIL_ALREADY_EXISTS'},
            True,
            id='email taken'
        ),
        pytest.param(
            {
                'username': 'demon_slayer',
                'email': 'simple@gmail.com',
                'password': '777cantfindme',
            },
            400,
            {'code': '400', 'message': 'USERNAME_ALREADY_EXISTS'},
            True,
            id='username taken'
        ),
        pytest.param(
            {
                'username': 'simple777',
                'email': 'invalid@email',
                'password': '777cantfindme',
            },
            500,
            {
                # this is bad, but I don't know how to override
                # errors thrown by chaotic's validation
                'code': '500',
                'details': "Error at path 'email': doesn't match regex",
                'message': 'Internal Server Error'
            },
            True,
            id='invalid email format'
        ),
        pytest.param(
            {
                'username': 'simple777',
                'email': 'simple@gmail.com',
                'password': 'findme',
            },
            500,
            {
                'code': '500',
                'details': "Error at path 'password': Too short string, minimum length=8, given=6",
                'message': 'Internal Server Error'
            },
            True,
            id='too short password'
        ),
        pytest.param(
            {
                'username': '*&^breakyou',
                'email': 'simple@gmail.com',
                'password': '777cantfindme',
            },
            500,
            {
                'code': '500',
                'details': "Error at path 'username': doesn't match regex",
                'message': 'Internal Server Error'
            },
            True,
            id='invalid chars in username'
        ),
    ],
)
async def test_register(
    service_client: Client,
    request_body,
    response_status,
    error_response_json,
    should_fail,
):
    response = await service_client.post(
        '/v1/auth/register',
        json=request_body
    )

    assert response.status == response_status
    if should_fail:
        assert response.json() == error_response_json
    else:
        assert 'id' in response.json()
