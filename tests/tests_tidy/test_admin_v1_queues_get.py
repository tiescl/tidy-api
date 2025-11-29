import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql', 'queues.sql'])
@pytest.mark.parametrize(
    "url, response_len, response_queues",
    [
        pytest.param(
            '/admin/v1/queues',
            9,
            [
                'TIDYBACK',
                'TIDYFRONT',
                'TIDYARCHREVIEW',
                'TIDYQA',
                'TIDYDUTY',
                'TEST',
                'TIDYTOP',
                'STUDYWITHME',
                'TRASH',
            ],
            id='ok all queues'
        ),
        pytest.param(
            '/admin/v1/queues?limit=1',
            1,
            ['TRASH'],
            id='one most recent'
        ),
        pytest.param(
            '/admin/v1/queues?page=9&limit=1',
            1,
            ['TIDYBACK'],
            id='one most ancient'
        ),
        pytest.param(
            '/admin/v1/queues?search=tidy',
            6,
            ['TIDYBACK', 'TIDYFRONT', 'TIDYQA',
                'TIDYDUTY', 'TIDYTOP', 'TIDYARCHREVIEW'],
            id='name contains tidy'
        ),
        pytest.param(
            '/admin/v1/queues?page=1123232142142412423412412341234124234214124134123412',
            -1,
            [],
            id='stupid value for page'
        )
    ],
)
async def test_admin_get_queues(
    service_client: Client,
    url,
    response_len,
    response_queues,
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
    for queue in queues:
        assert queue['key'] in response_queues
