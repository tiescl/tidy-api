import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql', 'queues.sql'])
@pytest.mark.parametrize(
    "url, response_len, response_json",
    [
        pytest.param(
            '/admin/v1/queues',
            9,
            'ok_all_queues_response.json',
            id='ok all queues'
        ),
        pytest.param(
            '/admin/v1/queues?limit=1',
            1,
            'one_most_recent_response.json',
            id='one most recent'
        ),
        pytest.param(
            '/admin/v1/queues?page=9&limit=1',
            1,
            'one_most_ancient_response.json',
            id='one most ancient'
        ),
        pytest.param(
            '/admin/v1/queues?search=tidy',
            6,
            'name_contains_tidy_response.json',
            id='name contains tidy'
        ),
        pytest.param(
            '/admin/v1/queues?page=1123232142142412423412412341234124234214124134123412',
            -1,
            None,
            id='stupid value for page'
        )
    ],
)
async def test_admin_get_queues(
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

    queues = response.json()['queues']
    assert len(queues) == response_len
    if response_json:
        assert queues == load_json(response_json)
