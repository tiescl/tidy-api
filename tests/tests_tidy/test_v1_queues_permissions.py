import typing
import pytest

from testsuite.daemons.service_client import Client
from testsuite.databases.pgsql.control import PgDatabaseWrapper

from tests.tests_tidy.consts import DB_NAME


def select_queue_role_permission(pgsql: typing.Dict[str, PgDatabaseWrapper], queue_id: str):
    cursor = pgsql[DB_NAME].cursor()
    cursor.execute("""
       SELECT
           queue_id,
           role,
           actions
       FROM tidy.queue_role_permissions
       WHERE queue_id = %s
    """, (queue_id,))

    return cursor.fetchone()


def select_queue_user_permission(pgsql: typing.Dict[str, PgDatabaseWrapper], queue_id: str):
    cursor = pgsql[DB_NAME].cursor()
    cursor.execute("""
       SELECT
           queue_id,
           user_id,
           actions
       FROM tidy.queue_user_permissions
       WHERE queue_id = %s
    """, (queue_id,))

    return cursor.fetchone()


@pytest.mark.pgsql(DB_NAME, files=['users.sql', 'tokens.sql', 'queues.sql'])
@pytest.mark.parametrize(
    "queue_id, session_token, permission_type, request_body, response_status, error_response_json, should_fail",
    [
        pytest.param(
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            'f37116c18a9345a0a2b5ea97fbc4e8f0',
            'role',
            {
                'permission_type': 'role',
                'role': 'developer',
                'actions': ['view'],
            },
            200,
            None,
            False,
            id='ok role'
        ),
        pytest.param(
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            'f37116c18a9345a0a2b5ea97fbc4e8f0',
            'user',
            {
                'permission_type': 'user',
                'user_id': '5c4b6cc6-10f9-4663-b473-3ffd8cc225f5',
                'actions': ['view', 'create'],
            },
            200,
            None,
            False,
            id='ok user'
        ),
        pytest.param(
            '6d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            'f37116c18a9345a0a2b5ea97fbc4e8f0',
            'role',
            {
                'permission_type': 'role',
                'role': 'developer',
                'actions': ['view'],
            },
            404,
            {'code': '404', 'message': 'QUEUE_NOT_FOUND'},
            True,
            id='queue not found'
        ),
        pytest.param(
            '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            'f47116c18a9345a0a2b5ea97fbc4e8f0',
            'user',
            {
                'permission_type': 'user',
                'user_id': '5c4b6cc6-10f9-4663-b473-3ffd8cc225f5',
                'actions': ['delete_any'],
            },
            400,
            {'code': '400', 'message': 'QUEUE_OWNER_MISMATCH'},
            True,
            id='queue owner mismatch'
        ),
        pytest.param(
            '7d854c28-c6eb-4ed4-b429-aaf006cea6b5',
            'f37116c18a9345a0a2b5ea97fbc4e8f0',
            'user',
            {
                'permission_type': 'user',
                'user_id': '5c4b6cc6-10f9-4663-b473-3ffd8cc225f5',
                'actions': ['view'],
            },
            404,
            {'code': '404', 'message': 'QUEUE_NOT_FOUND'},
            True,
            id='deleted queue'
        ),
    ],
)
async def test_create_queue_permissions(
    service_client: Client,
    pgsql: typing.Dict,
    queue_id: str,
    session_token: str,
    permission_type: str,
    request_body: typing.Dict,
    response_status: int,
    error_response_json: typing.Dict,
    should_fail: bool,
):
    response = await service_client.post(
        f'/v1/queues/{queue_id}/permissions',
        headers={'Cookie': f'session_token={session_token}'},
        json=request_body,
    )

    assert response.status == response_status
    if should_fail:
        assert response.json() == error_response_json
    else:
        if permission_type == 'role':
            assert select_queue_role_permission(pgsql, queue_id) == (
                '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
                'developer',
                '{view}',
            )
        else:
            assert select_queue_user_permission(pgsql, queue_id) == (
                '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
                '5c4b6cc6-10f9-4663-b473-3ffd8cc225f5',
                '{view,create}',
            )
