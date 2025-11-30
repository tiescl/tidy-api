import typing
import pytest

from testsuite.daemons.service_client import Client
from testsuite.databases.pgsql.control import PgDatabaseWrapper

from tests.tests_tidy.consts import DB_NAME


def select_removed_status(pgsql: typing.Dict[str, PgDatabaseWrapper], issue_id: str):
    cursor = pgsql[DB_NAME].cursor()

    cursor.execute("""
        SELECT removed
        FROM tidy.issues
        WHERE id = %s
    """, (issue_id, ))

    return cursor.fetchone()


@pytest.mark.pgsql(DB_NAME, files=[
    'users.sql',
    'tokens.sql',
    'queues.sql',
    'issues.sql',
])
@pytest.mark.parametrize(
    (
        'queue_id',
        'issue_id',
        'response_status',
        'should_fail',
        'error_response_json',
    ),
    [
        pytest.param(
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            '264c7fc0-6020-48ae-b10c-d0d1479d9355',
            200,
            False,
            None,
            id='ok owner'
        ),
        pytest.param(
            '1e6fd243-6239-4d76-8f0d-2bcdbc194f18',
            'fb18e7f4-2cfd-4cf8-bbf0-ab575f2a1e6c',
            200,
            False,
            None,
            id='ok delete own perm'
        ),
        pytest.param(
            '57996a05-fb8c-454d-910e-8b53d313741e',
            '4dbef3e1-011a-4536-9473-42b414e1bf79',
            200,
            False,
            None,
            id='ok delete any perm'
        ),
        pytest.param(
            '1e6fd243-6239-4d76-8f0d-2bcdbc194f18',
            '264c7fc0-6020-48ae-b10c-d0d1479d9355',
            404,
            True,
            {'code': '404', 'message': 'ISSUE_NOT_FOUND'},
            id='issue belongs in another queue'
        ),
        pytest.param(
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            '076a4461-5faf-41c3-b061-de510fc2d409',
            404,
            True,
            {'code': '404', 'message': 'ISSUE_NOT_FOUND'},
            id='issue removed'
        ),
        pytest.param(
            '5bf11df6-3c74-4962-a272-aed7c9e35f22',
            '5fe28b12-824e-4c0a-aee4-f98e9b9aacbc',
            404,
            True,
            {'code': '404', 'message': 'QUEUE_NOT_FOUND'},
            id='queue removed'
        ),
        pytest.param(
            '1e6fd243-6239-4d76-8f0d-2bcdbc194f18',
            'ed77ea7c-99b0-47c7-8f8d-936d2aced769',
            403,
            True,
            {'code': '403', 'message': 'FORBIDDEN'},
            id='no perms'
        )
    ],
)
async def test_delete_issue(
    service_client: Client,
    pgsql,
    queue_id,
    issue_id,
    response_status,
    should_fail,
    error_response_json,
):
    response = await service_client.post(
        f'/v1/queues/{queue_id}/issues/{issue_id}/delete',
        headers={'Cookie': 'session_token=f37116c18a9345a0a2b5ea97fbc4e8f0'}
    )

    assert response.status == response_status
    if should_fail:
        assert response.json() == error_response_json
    else:
        assert select_removed_status(pgsql, issue_id) == (True,)
