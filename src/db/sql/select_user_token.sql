-- $1 - token

SELECT
    token,
    user_id,
    user_role,
    expires_at,
    increment
FROM tidy.user_tokens
WHERE
    token = $1
    AND expires_at >= NOW();
