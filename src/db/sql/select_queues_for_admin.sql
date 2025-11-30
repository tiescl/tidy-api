-- $1 - search term
-- $2 - limit
-- $3 - offset

SELECT
    id,
    key,
    name,
    ROW (
        owner_id,
        (SELECT username FROM tidy.users WHERE id = owner_id)
    ) AS owner,
    removed,
    EXTRACT(epoch FROM created_at)::BIGINT,
    EXTRACT(epoch FROM updated_at)::BIGINT
FROM tidy.queues
WHERE
    CASE
        WHEN $1 = '' THEN TRUE
        ELSE key ILIKE $1 OR name ILIKE $1
    END
ORDER BY created_at DESC
LIMIT $2
OFFSET $3;
