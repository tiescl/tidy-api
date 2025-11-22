-- $1 - user_id

SELECT
    password_hash
FROM tidy.users
WHERE
    id = $1
    AND NOT removed;
