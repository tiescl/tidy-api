-- $1 - user_id
-- $2 - new_password_hash

UPDATE tidy.users
SET password_hash = $2
WHERE id = $1;
