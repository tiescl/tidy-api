-- $1 = queue_id
-- $2 = user_id
-- $3 = owner_id

WITH queue AS (
    SELECT owner_id
    FROM tidy.queues
    WHERE
        id = $1
        AND NOT removed
),
authorized AS (
    SELECT 1
    FROM queue
    WHERE owner_id = $3
),
delete_attempt AS (
    DELETE FROM tidy.queue_user_permissions
    WHERE
        queue_id = $1
        AND user_id = $2
        AND EXISTS (SELECT 1 FROM authorized)
    RETURNING 1
)
SELECT CASE
    WHEN NOT EXISTS (SELECT 1 FROM queue)
        THEN 'QUEUE_NOT_FOUND'
    WHEN NOT EXISTS (SELECT 1 FROM authorized)
        THEN 'QUEUE_OWNER_MISMATCH'
    ELSE 'OK'
END AS code;
