-- $1 - user_id
-- $2 - full_name
-- $3 - email
-- $4 - username

UPDATE tidy.users
SET
    full_name = COALESCE($2, full_name),
    email = COALESCE($3, email),
    username = COALESCE($4, username)
WHERE
    id = $1
    AND NOT removed
RETURNING
    id,
    full_name,
    email,
    username,
    role;
