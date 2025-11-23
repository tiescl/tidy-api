-- $1 - queue_id
-- $2 - role
-- $3 - actions
-- $4 - owner_id

WITH queue AS (
    SELECT owner_id
    FROM tidy.queues
    WHERE
        id = $1
        AND NOT removed
),
insert_attempt AS (
    INSERT INTO tidy.queue_role_permissions (
        queue_id,
        role,
        actions
    )
    SELECT $1, $2, $3
    FROM queue
    WHERE queue.owner_id = $4
    ON CONFLICT (queue_id, role) DO UPDATE
        SET actions = EXCLUDED.actions
    RETURNING 'OK' AS result
)
SELECT
    CASE
        WHEN NOT EXISTS (SELECT 1 FROM queue)
            THEN 'QUEUE_NOT_FOUND'
        WHEN EXISTS (SELECT 1 FROM queue WHERE owner_id != $4)
            THEN 'QUEUE_OWNER_MISMATCH'
        WHEN EXISTS (SELECT 1 FROM insert_attempt)
            THEN 'OK'
    END;
