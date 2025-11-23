-- $1 - key
-- $2 - name
-- $3 - owner_id

INSERT INTO tidy.queues (
    key,
    name,
    owner_id
)
SELECT $1, $2, $3
WHERE EXISTS (
    SELECT 1
    FROM tidy.users
    WHERE
        users.id = $3
        AND NOT users.removed
)
RETURNING id;
