import typing
import pytest

from testsuite.daemons.service_client import Client
from testsuite.databases.pgsql.control import PgDatabaseWrapper

from tests.tests_tidy.consts import DB_NAME


def select_updated_password(pgsql: typing.Dict[str, PgDatabaseWrapper]):
    cursor = pgsql[DB_NAME].cursor()
    cursor.execute("""
        SELECT password_hash
        FROM tidy.users
        WHERE id = '774cc050-2542-4c05-9027-f4919112fc81'
    """)

    return cursor.fetchone()


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql'])
async def test_user_set_new_password(service_client: Client, pgsql):
    old_hash = select_updated_password(pgsql)

    response = await service_client.put(
        f'/v1/users/me/password',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'},
        json={
            'old_password': '777cantfindme',
            'new_password': 'successful_hacking_attempt',
        }
    )

    new_hash = select_updated_password(pgsql)

    assert response.status == 200
    assert new_hash != old_hash
