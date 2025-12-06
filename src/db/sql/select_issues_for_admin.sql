-- $1 - search term
-- $2 - limit
-- $3 - offset

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
    issue.removed,
    ROW (
        author.id,
        author.username,
        author.removed
    ),
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
LEFT JOIN tidy.queues queue
    ON queue.id = issue.queue_id
LEFT JOIN tidy.users author
    ON author.id = issue.author_id
LEFT JOIN tidy.users assignee
    ON assignee.id = issue.assignee_id
WHERE (
    $1 = ''
    OR queue.key ILIKE $1
    OR issue.title ILIKE $1
    OR issue.description ILIKE $1
)
ORDER BY
    issue.updated_at DESC
LIMIT $2
OFFSET $3;
