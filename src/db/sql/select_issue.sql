-- $1  - user_id
-- $2  - queue_id
-- $3  - issue_id

WITH user_check AS (
    SELECT id, username, role
    FROM tidy.users
    WHERE
        id = $1
        AND NOT removed
),

queue_check AS (
    SELECT id, key, owner_id
    FROM tidy.queues
    WHERE
        id = $2
        AND NOT removed
),

permission_check AS (
    SELECT queue.id AS queue_id
    FROM queue_check queue
    JOIN user_check auth_user ON TRUE

    LEFT JOIN tidy.queue_user_permissions u
        ON u.queue_id = queue.id AND u.user_id = $1

    LEFT JOIN tidy.queue_role_permissions r
        ON r.queue_id = queue.id AND r.role = auth_user.role

    WHERE
        queue.owner_id = $1
        OR (u.actions @> ARRAY['view']::tidy.issue_action[])
        OR (r.actions @> ARRAY['view']::tidy.issue_action[])
),

issue_check AS (
    SELECT
        id,
        queue_id,
        number,
        title,
        description,
        type,
        status,
        priority,
        component,
        story_points,
        author_id,
        assignee_id,
        created_at,
        updated_at
    FROM tidy.issues
    WHERE
        id = $3
        AND queue_id = $2
        AND NOT removed
)
SELECT
    CASE
        WHEN NOT EXISTS (SELECT 1 FROM user_check)
            THEN 'USER_NOT_FOUND'
        WHEN NOT EXISTS (SELECT 1 FROM queue_check)
            THEN 'QUEUE_NOT_FOUND'
        WHEN NOT EXISTS (SELECT 1 FROM permission_check)
            THEN 'FORBIDDEN'
        WHEN NOT EXISTS (SELECT 1 FROM issue_check)
            THEN 'ISSUE_NOT_FOUND'
        ELSE 'OK'
    END AS code,
    (
        -- order MUST match issues.yaml#/components/schemas/Issue
        SELECT ROW (
            issue.id,
            ROW (
                queue.id,
                queue.key
            ),
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
                author.username
            ),
            CASE
                WHEN issue.assignee_id IS NULL OR assignee.username IS NULL
                    THEN NULL
                ELSE ROW (
                    assignee.id,
                    assignee.username
                )
            END,
            EXTRACT(EPOCH FROM issue.created_at)::BIGINT,
            EXTRACT(EPOCH FROM issue.updated_at)::BIGINT
        )
        FROM issue_check issue
        INNER JOIN queue_check queue
            ON queue.id = issue.queue_id
        INNER JOIN tidy.users author
            ON author.id = issue.author_id
        LEFT JOIN tidy.users assignee
            ON assignee.id = issue.assignee_id
    ) AS issue;
