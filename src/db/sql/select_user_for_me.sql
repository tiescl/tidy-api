-- $1 - user_id

SELECT
    id,
    full_name,
    email,
    username,
    role
FROM tidy.users
WHERE 
    id = $1
    AND NOT removed;
