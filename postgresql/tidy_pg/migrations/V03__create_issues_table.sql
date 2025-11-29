DROP TYPE IF EXISTS tidy.issue_status;
CREATE TYPE tidy.issue_status AS ENUM (
    'description',
    'backlog',
    'planned',
    'in_progress',
    'need_info',
    'pull_request',
    'merged',
    'closed'
);

DROP TYPE IF EXISTS tidy.issue_priority;
CREATE TYPE tidy.issue_priority AS ENUM (
    'minor',
    'normal',
    'critical',
    'blocker'
);

DROP TYPE IF EXISTS tidy.issue_type;
CREATE TYPE tidy.issue_type AS ENUM (
    'task',
    'bug',
    'linker',
    'epic'
);

CREATE TABLE IF NOT EXISTS tidy.issues (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    queue_id UUID NOT NULL
        REFERENCES tidy.queues(id) ON DELETE RESTRICT,
    number INTEGER NOT NULL,

    title TEXT NOT NULL,
    description TEXT,
    type tidy.issue_type NOT NULL DEFAULT 'task',
    status tidy.issue_status NOT NULL DEFAULT 'description',
    priority tidy.issue_priority NOT NULL DEFAULT 'normal',

    component TEXT,
    story_points INTEGER,

    author_id UUID NOT NULL
        REFERENCES tidy.users(id) ON DELETE RESTRICT,
    assignee_id UUID
        REFERENCES tidy.users(id) ON DELETE RESTRICT,

    removed BOOLEAN NOT NULL DEFAULT false,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    UNIQUE (queue_id, number)
);

DROP TYPE IF EXISTS tidy.issue_link_type;
CREATE TYPE tidy.issue_link_type AS ENUM (
    'parent',
    'relates_to'
);

CREATE TABLE IF NOT EXISTS tidy.issue_links (
    from_issue_id UUID NOT NULL
        REFERENCES tidy.issues(id) ON DELETE RESTRICT,
    to_issue_id UUID NOT NULL
        REFERENCES tidy.issues(id) ON DELETE RESTRICT,
    link_type tidy.issue_link_type NOT NULL,

    PRIMARY KEY (from_issue_id, to_issue_id, link_type)
);
