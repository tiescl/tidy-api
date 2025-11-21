-- $1 - email

SELECT
    id,
    email,
    password_hash
FROM tidy.users
WHERE email = $1;
