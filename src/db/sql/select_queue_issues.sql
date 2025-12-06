-- $1 - user_id
-- $2 - queue_id
-- $3 - search
-- $4 - limit
-- $5 - offset

WITH user_role AS (
    SELECT role
    FROM tidy.users
    WHERE id = $1
),

queue AS (
    SELECT
        q.id,
        q.key,
        q.name,
        ROW (
            q.owner_id,
            users.username,
            users.removed
        ) AS owner,
        EXTRACT(epoch FROM q.created_at)::BIGINT AS created_at
    FROM tidy.queues q
    LEFT JOIN tidy.users
        ON users.id = q.owner_id
    LEFT JOIN tidy.queue_user_permissions u
        ON u.queue_id = q.id AND u.user_id = $1
    LEFT JOIN tidy.queue_role_permissions r
        ON r.queue_id = q.id AND r.role = (SELECT role FROM user_role)
    WHERE
        q.id = $2
        AND NOT q.removed
        AND (
            q.owner_id = $1
            OR (u.actions @> ARRAY['view']::tidy.issue_action[])
            OR (r.actions @> ARRAY['view']::tidy.issue_action[])
        )
),

issues AS (
    SELECT
        issue.id,
        ROW (
            queue.id,
            queue.key
        ) AS queue,
        issue.number,
        issue.title,
        issue.description,
        issue.type,
        issue.status,
        issue.priority,
        issue.component,
        issue.story_points,
        ROW (
            author.id,
            author.username,
            author.removed
        ) AS author,
        CASE
            WHEN assignee.id IS NULL THEN NULL
            ELSE ROW (
                assignee.id,
                assignee.username,
                assignee.removed
            )
        END AS assignee,
        EXTRACT(EPOCH FROM issue.created_at)::BIGINT AS created_at,
        EXTRACT(EPOCH FROM issue.updated_at)::BIGINT AS updated_at
    FROM tidy.issues issue
    INNER JOIN queue
        ON queue.id = issue.queue_id
    LEFT JOIN tidy.users author
        ON author.id = issue.author_id
    LEFT JOIN tidy.users assignee
        ON assignee.id = issue.assignee_id
    WHERE
        CASE
            WHEN $3 = '' THEN TRUE
            ELSE issue.title ILIKE $3 OR issue.description ILIKE $3
        END
        AND NOT issue.removed
    ORDER BY issue.updated_at DESC
    LIMIT $4
    OFFSET $5
)

SELECT
    queue,
    COALESCE(
        (SELECT array_agg(issues) FROM issues),
        '{}'
    ) AS issues
FROM queue;
