-- $1 - user_id
-- $2 - user_role

WITH update_tokens AS (
    UPDATE tidy.user_tokens
    SET user_role = $2
    WHERE user_id = $1
)
UPDATE tidy.users
SET role = $2
WHERE id = $1;
