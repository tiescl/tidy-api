-- $1 - search term
-- $2 - limit
-- $3 - offset

SELECT
    q.id,
    q.key,
    q.name,
    ROW (
        q.owner_id,
        u.username,
        u.removed
    ) AS owner,
    q.removed,
    EXTRACT(epoch FROM q.created_at)::BIGINT,
    EXTRACT(epoch FROM q.updated_at)::BIGINT
FROM tidy.queues q
LEFT JOIN tidy.users u
    ON q.owner_id = u.id
WHERE
    CASE
        WHEN $1 = '' THEN TRUE
        ELSE q.key ILIKE $1 OR q.name ILIKE $1
    END
ORDER BY q.created_at DESC
LIMIT $2
OFFSET $3;
