-- $1 - user_id
-- $2 - search term
-- $3 - limit
-- $4 - offset

WITH user_role AS (
    SELECT role
    FROM tidy.users
    WHERE id = $1
)
SELECT DISTINCT
    q.id,
    q.key,
    q.name,
    ROW (
        q.owner_id,
        users.username,
        users.removed
    ) AS owner,
    EXTRACT(epoch FROM q.created_at)::BIGINT
FROM tidy.queues q
LEFT JOIN tidy.users
    ON users.id = q.owner_id
LEFT JOIN tidy.queue_user_permissions u
    ON u.queue_id = q.id
       AND u.user_id = $1
LEFT JOIN tidy.queue_role_permissions r
    ON r.queue_id = q.id
       AND r.role = (SELECT role FROM user_role)
WHERE
    (
        q.owner_id = $1
        OR (u.actions @> ARRAY['view']::tidy.issue_action[])
        OR (r.actions @> ARRAY['view']::tidy.issue_action[])
    )
    AND CASE
        WHEN $2 = '' THEN TRUE
        ELSE q.key ILIKE $2 OR q.name ILIKE $2
    END
    AND NOT q.removed
ORDER BY EXTRACT(epoch FROM q.created_at)::BIGINT DESC
LIMIT $3
OFFSET $4;
