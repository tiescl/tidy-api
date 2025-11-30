import pytest

from testsuite.daemons.service_client import Client

from tests.tests_tidy.consts import DB_NAME


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql', 'queues.sql'])
async def test_admin_delete_queues(service_client: Client):
    response = await service_client.delete(
        '/admin/v1/queues',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'},
        json={'queue_ids': ['5d854c28-c6eb-4ed4-b429-aaf006cea6b5', '6d854c28-c6eb-4ed4-b429-aaf006cea6b5']}
    )

    assert response.status == 200
