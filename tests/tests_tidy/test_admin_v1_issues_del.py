import typing
import pytest

from testsuite.daemons.service_client import Client
from testsuite.databases.pgsql.control import PgDatabaseWrapper

from tests.tests_tidy.consts import DB_NAME


def select_count_removed_issues(pgsql: typing.Dict[str, PgDatabaseWrapper]):
    cursor = pgsql[DB_NAME].cursor()

    cursor.execute("""
        SELECT COUNT(*)
        FROM tidy.issues
        WHERE removed = TRUE
    """)

    return cursor.fetchone()


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql', 'queues.sql', 'issues.sql'])
async def test_admin_delete_issues(service_client: Client, pgsql):
    assert select_count_removed_issues(pgsql) == (0,)

    response = await service_client.post(
        '/admin/v1/issues/delete',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'},
        json={'issue_ids': ['264c7fc0-6020-48ae-b10c-d0d1479d9355',
                            '5fe28b12-824e-4c0a-aee4-f98e9b9aacbc',
                            '18225480-acaa-4521-a5e2-bc8ba43725e0']}
    )

    assert response.status == 200
    assert select_count_removed_issues(pgsql) == (3,)
