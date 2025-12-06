-- $1  - user_id
-- $2  - queue_id
-- $3  - issue_id
-- $4  - title (opt)
-- $5  - description (opt)
-- $6  - type
-- $7  - status
-- $8  - priority
-- $9  - component (opt)
-- $10 - story_points (opt)
-- $11 - assignee_id (opt)

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

issue_check AS (
    SELECT id, author_id, assignee_id
    FROM tidy.issues
    WHERE
        id = $3
        AND queue_id = $2
        AND NOT removed
),

assignee_check AS (
    SELECT
        id,
        username,
        removed
    FROM tidy.users
    WHERE
        id = $11
        AND NOT removed
),

permission_check AS (
    SELECT TRUE AS has_permission
    FROM queue_check queue
    CROSS JOIN user_check auth_user
    CROSS JOIN issue_check issue

    LEFT JOIN tidy.queue_user_permissions u
        ON u.queue_id = queue.id AND u.user_id = $1
    LEFT JOIN tidy.queue_role_permissions r
        ON r.queue_id = queue.id AND r.role = auth_user.role
    WHERE
        queue.owner_id = $1
        OR (
            issue.author_id = $1
            AND (
                u.actions @> ARRAY['edit_own']::tidy.issue_action[]
                OR r.actions @> ARRAY['edit_own']::tidy.issue_action[]
            )
        )
        OR (
            u.actions @> ARRAY['edit_any']::tidy.issue_action[]
            OR r.actions @> ARRAY['edit_any']::tidy.issue_action[]
        )
),

update_attempt AS (
    UPDATE tidy.issues
    SET
        title = COALESCE($4, title),
        description = COALESCE($5, description),
        type = COALESCE($6, type),
        status = COALESCE($7, status),
        priority = COALESCE($8, priority),
        component = COALESCE($9, component),
        story_points = COALESCE($10, story_points),
        assignee_id = COALESCE($11, assignee_id),
        updated_at = NOW()
    WHERE
        id = $3
        AND EXISTS (SELECT 1 FROM permission_check)
    RETURNING *
)

SELECT
    CASE
        WHEN NOT EXISTS (SELECT 1 FROM user_check)
            THEN 'USER_NOT_FOUND'
        WHEN NOT EXISTS (SELECT 1 FROM queue_check)
            THEN 'QUEUE_NOT_FOUND'
        WHEN NOT EXISTS (SELECT 1 FROM issue_check)
            THEN 'ISSUE_NOT_FOUND'
        WHEN $11 IS NOT NULL
             AND NOT EXISTS (SELECT 1 FROM assignee_check)
            THEN 'USER_NOT_FOUND'
        WHEN NOT EXISTS (SELECT 1 FROM permission_check)
            THEN 'FORBIDDEN'
        WHEN EXISTS (SELECT 1 FROM update_attempt)
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
                author.username,
                author.removed
            ),
            CASE
                WHEN assignee.id IS NULL OR assignee.username IS NULL
                    THEN NULL
                ELSE ROW (
                    assignee.id,
                    assignee.username,
                    assignee.removed
                )
            END,
            EXTRACT(EPOCH FROM issue.created_at)::BIGINT,
            EXTRACT(EPOCH FROM issue.updated_at)::BIGINT
        )
        FROM update_attempt issue
        CROSS JOIN queue_check queue
        CROSS JOIN permission_check perm
        INNER JOIN tidy.users author
            ON author.id = issue.author_id
        LEFT JOIN assignee_check assignee
            ON assignee.id = issue.assignee_id
    ) AS issue;
