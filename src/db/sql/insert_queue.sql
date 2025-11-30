-- $1 - key
-- $2 - name
-- $3 - owner_id

WITH owner AS (
    SELECT id, username
    FROM tidy.users
    WHERE
        id = $3
        AND NOT removed
),
insert_attempt AS (
    INSERT INTO tidy.queues (
        key,
        name,
        owner_id
    )
    SELECT $1, $2, owner.id
    FROM owner
    RETURNING
        id,
        key,
        name,
        owner_id,
        created_at
)
SELECT
    i.id,
    i.key,
    i.name,
    ROW (
        i.owner_id,
        o.username
    ) AS owner,
    EXTRACT(epoch FROM i.created_at)::BIGINT
FROM insert_attempt i
CROSS JOIN owner o;
