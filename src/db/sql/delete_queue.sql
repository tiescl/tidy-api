-- $1 - queue_id
-- $2 - owner_id

WITH updated AS (
    UPDATE tidy.queues
    SET removed = TRUE
    WHERE id = $1 AND owner_id = $2
    RETURNING id
),
queue_check AS (
    SELECT id
    FROM tidy.queues
    WHERE id = $1
)
SELECT
    CASE
        WHEN u.id IS NOT NULL THEN 'OK'
        WHEN q.id IS NULL THEN 'QUEUE_NOT_FOUND'
        ELSE 'QUEUE_OWNER_MISMATCH'
    END AS result
FROM (SELECT $1 AS id) AS v
LEFT JOIN queue_check q ON q.id = v.id
LEFT JOIN updated u ON u.id = v.id;
