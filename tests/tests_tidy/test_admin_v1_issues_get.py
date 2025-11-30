import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql', 'queues.sql', 'issues.sql'])
@pytest.mark.parametrize(
    "url, response_len, response_json",
    [
        pytest.param(
            '/admin/v1/issues',
            5,
            'ok_all_issues_response.json',
            id='ok all issues'
        ),
        pytest.param(
            '/admin/v1/issues?limit=1',
            1,
            'one_most_recent_response.json',
            id='one most recent'
        ),
        pytest.param(
            '/admin/v1/issues?page=5&limit=1',
            1,
            'one_most_ancient_response.json',
            id='one most ancient'
        ),
        pytest.param(
            '/admin/v1/issues?limit=1123232142142412423412412341234124234214124134123412',
            -1,
            None,
            id='stupid value for limit'
        ),
        pytest.param(
            '/admin/v1/issues?search=bugfix',
            2,
            'name_contains_bugfix_response.json',
            id='name contains bugfix'
        ),
    ],
)
async def test_admin_get_issues(
    service_client: Client,
    load_json,
    url,
    response_len,
    response_json,
):
    response = await service_client.get(
        url,
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'}
    )

    if response_len == -1:
        assert response.json() == {'code': '400',
                                   'message': 'INVALID_QUERY_PARAMS'}
        return

    assert response.status == 200

    issues = response.json()['issues']
    assert len(issues) == response_len
    print(issues)
    assert issues == load_json(response_json)
