-- $1  - queue_id
-- $2  - author_id
-- $3  - assignee_id (opt)
-- $4  - title
-- $5  - description (opt)
-- $6  - type (opt)
-- $7  - status (opt)
-- $8  - priority (opt)
-- $9  - component (opt)
-- $10 - story_points (opt)

WITH queue_check AS (
    SELECT id, key, owner_id
    FROM tidy.queues
    WHERE
        id = $1
        AND NOT removed
),

author_check AS (
    SELECT id, username, role
    FROM tidy.users
    WHERE
        id = $2
        AND NOT removed
),

assignee_check AS (
    SELECT id, username
    FROM tidy.users
    WHERE
        id = $3
        AND NOT removed
),

permission_check AS (
    SELECT TRUE AS has_permission
    FROM queue_check queue
    CROSS JOIN author_check author

    LEFT JOIN tidy.queue_user_permissions u
        ON u.queue_id = queue.id AND u.user_id = $2

    LEFT JOIN tidy.queue_role_permissions r
        ON r.queue_id = queue.id AND r.role = author.role

    WHERE
        queue.owner_id = $2
        OR (u.actions @> ARRAY['create']::tidy.issue_action[])
        OR (r.actions @> ARRAY['create']::tidy.issue_action[])
),

insert_attempt AS (
    INSERT INTO tidy.issues (
        queue_id,
        number,
        author_id,
        assignee_id,
        title,
        description,
        type,
        status,
        priority,
        component,
        story_points
    )
    SELECT
        queue.id,
        COALESCE(MAX(issues.number), 0) + 1,
        author.id,
        assignee.id,
        $4,                          -- title
        $5,                          -- description
        COALESCE($6, 'task'),        -- type
        COALESCE($7, 'description'), -- status
        COALESCE($8, 'normal'),      -- priority
        $9,                          -- component
        $10                          -- story_points
    FROM queue_check queue
    CROSS JOIN author_check author
    LEFT JOIN assignee_check assignee ON TRUE
    LEFT JOIN tidy.issues
        ON issues.queue_id = queue.id
    WHERE EXISTS (SELECT 1 FROM permission_check)
    GROUP BY
        queue.id,
        author.id,
        assignee.id
    RETURNING *
)

SELECT
    CASE
        WHEN NOT EXISTS (SELECT 1 FROM queue_check)
            THEN 'QUEUE_NOT_FOUND'
        WHEN NOT EXISTS (SELECT 1 FROM author_check)
            THEN 'USER_NOT_FOUND'
        WHEN $3 IS NOT NULL
             AND NOT EXISTS (SELECT 1 FROM assignee_check)
            THEN 'USER_NOT_FOUND'
        WHEN NOT EXISTS (SELECT 1 FROM permission_check)
            THEN 'FORBIDDEN'
        WHEN EXISTS (SELECT 1 FROM insert_attempt)
            THEN 'OK'
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
                WHEN assignee.id IS NULL OR assignee.username IS NULL
                    THEN NULL
                ELSE ROW (
                    assignee.id,
                    assignee.username
                )
            END,
            EXTRACT(EPOCH FROM issue.created_at)::BIGINT,
            EXTRACT(EPOCH FROM issue.updated_at)::BIGINT
        )
        FROM insert_attempt issue
        CROSS JOIN queue_check queue
        CROSS JOIN author_check author
        LEFT JOIN assignee_check assignee
            ON assignee.id = issue.assignee_id
        LIMIT 1
    ) AS issue;
