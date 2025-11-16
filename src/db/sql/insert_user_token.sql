-- $1 - token
-- $2 - user_id
-- $3 - user_role
-- $4 - expires_at
-- $5 - updated_at

INSERT INTO tidy.user_tokens (
    token,
    user_id,
    user_role,
    expires_at,
    updated_at
)
VALUES ($1, $2, $3, $4, $5);
