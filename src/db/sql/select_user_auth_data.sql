-- $1 - email

SELECT
    id,
    email,
    role,
    password_hash
FROM tidy.users
WHERE
    email = $1
    AND NOT removed;
