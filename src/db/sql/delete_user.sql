-- $1 - user_id

WITH update_tokens AS (
    UPDATE tidy.user_tokens
    SET expires_at = (NOW() - '1s'::INTERVAL)
    WHERE user_id = $1
)
UPDATE tidy.users
SET removed = TRUE
WHERE id = $1;
