-- $1 - token
-- $2 - user_id
-- $3 - user_role
-- $4 - expires_at

INSERT INTO tidy.user_tokens (
    token,
    user_id,
    user_role,
    expires_at
)
VALUES ($1, $2, $3, $4);
