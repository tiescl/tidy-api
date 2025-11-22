-- $1 - token
-- $2 - expire_time

UPDATE tidy.user_tokens
SET expires_at = $2
WHERE token = $1;
