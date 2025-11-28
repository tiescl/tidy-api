-- $1 = user_id
-- $2 = queue_id

WITH user_role AS (
    SELECT role
    FROM tidy.users
    WHERE id = $1
)
SELECT
    q.id,
    q.key,
    q.name,
    q.owner_id,
    EXTRACT(epoch FROM q.created_at)::BIGINT
FROM tidy.queues q
LEFT JOIN tidy.queue_user_permissions u
    ON u.queue_id = q.id
       AND u.user_id = $1
LEFT JOIN tidy.queue_role_permissions r
    ON r.queue_id = q.id
       AND r.role = (SELECT role FROM user_role)
WHERE
    q.id = $2
    AND (
        q.owner_id = $1
        OR (u.actions @> ARRAY['view']::tidy.issue_action[])
        OR (r.actions @> ARRAY['view']::tidy.issue_action[])
    )
    AND NOT q.removed;
