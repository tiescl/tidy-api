-- $1 - search
-- $2 - limit
-- $3 - offset

SELECT
    id,
    full_name,
    username,
    email,
    role,
    removed,
    EXTRACT(epoch FROM created_at)::BIGINT,
    EXTRACT(epoch FROM updated_at)::BIGINT
FROM tidy.users
WHERE
    CASE
        WHEN $1 = '' THEN TRUE
        ELSE email ILIKE $1 OR username ILIKE $1
    END
ORDER BY created_at DESC
LIMIT $2
OFFSET $3;
