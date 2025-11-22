-- $1 - user_id

UPDATE tidy.users
SET removed = TRUE
WHERE id = $1;
