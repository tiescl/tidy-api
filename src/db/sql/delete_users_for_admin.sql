-- $1 - user_ids

WITH update_tokens AS (
    UPDATE tidy.user_tokens
    SET expires_at = (NOW() - '1s'::INTERVAL)
    WHERE user_id = ANY($1)
)
UPDATE tidy.users
SET removed = TRUE
WHERE id = ANY($1);
