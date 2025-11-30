-- $1 - user_id
-- $2 - queue_id
-- $3 - issue_id

WITH user_check AS (
    SELECT id, role
    FROM tidy.users
    WHERE
        id = $1
        AND NOT removed
),

queue_check AS (
    SELECT id, owner_id
    FROM tidy.queues
    WHERE
        id = $2
        AND NOT removed
),

issue_check AS (
    SELECT id, author_id
    FROM tidy.issues
    WHERE
        id = $3
        AND queue_id = $2
        AND NOT removed
),

permission_check AS (
    SELECT TRUE AS has_permission
    FROM queue_check queue
    CROSS JOIN user_check auth_user
    CROSS JOIN issue_check issue

    LEFT JOIN tidy.queue_user_permissions u
        ON u.queue_id = queue.id AND u.user_id = $1
    LEFT JOIN tidy.queue_role_permissions r
        ON r.queue_id = queue.id AND r.role = auth_user.role
    WHERE
        queue.owner_id = $1
        OR (
            issue.author_id = $1
            AND (
                u.actions @> ARRAY['delete_own']::tidy.issue_action[]
                OR r.actions @> ARRAY['delete_own']::tidy.issue_action[]
            )
        )
        OR (
            u.actions @> ARRAY['delete_any']::tidy.issue_action[]
            OR r.actions @> ARRAY['delete_any']::tidy.issue_action[]
        )
),

delete_attempt AS (
    UPDATE tidy.issues
    SET removed = TRUE
    WHERE
        id = $3
        AND EXISTS (SELECT 1 FROM permission_check)
)

SELECT CASE
    WHEN NOT EXISTS (SELECT 1 FROM user_check)
        THEN 'USER_NOT_FOUND'
    WHEN NOT EXISTS (SELECT 1 FROM queue_check)
        THEN 'QUEUE_NOT_FOUND'
    WHEN NOT EXISTS (SELECT 1 FROM issue_check)
        THEN 'ISSUE_NOT_FOUND'
    WHEN NOT EXISTS (SELECT 1 FROM permission_check)
        THEN 'FORBIDDEN'
    ELSE 'OK'
END AS code;
