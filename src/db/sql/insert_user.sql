-- $1 - username
-- $2 - email
-- $3 - password_hash
-- $4 - role

INSERT INTO tidy.users (
    username,
    email,
    password_hash,
    role
)
VALUES ($1, $2, $3, $4)
RETURNING id;
