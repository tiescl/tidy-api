CREATE SCHEMA IF NOT EXISTS tidy;

DROP TYPE IF EXISTS tidy.user_role CASCADE;
CREATE TYPE tidy.user_role AS ENUM (
    'pending',
    'admin',
    'manager',
    'recruiter',
    'developer'
);

CREATE TABLE IF NOT EXISTS tidy.users (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    full_name TEXT,
    email TEXT NOT NULL,
    username TEXT NOT NULL,
    password_hash TEXT NOT NULL,
    role tidy.user_role NOT NULL,
    removed BOOLEAN NOT NULL DEFAULT false,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    increment BIGSERIAL UNIQUE NOT NULL,

    CONSTRAINT users_unique_email UNIQUE (email),
    CONSTRAINT users_unique_username UNIQUE (username)
);

CREATE TABLE IF NOT EXISTS tidy.user_tokens (
    token TEXT PRIMARY KEY NOT NULL,
    user_id UUID NOT NULL
        REFERENCES tidy.users(id) ON DELETE RESTRICT,
    user_role tidy.user_role NOT NULL,
    expires_at TIMESTAMPTZ NOT NULL,
    increment BIGSERIAL UNIQUE NOT NULL
);

CREATE INDEX IF NOT EXISTS idx_users_search ON tidy.users (email, username);
CREATE INDEX IF NOT EXISTS idx_user_tokens_user_id ON tidy.user_tokens (user_id);

-- triggers
CREATE OR REPLACE FUNCTION set_update()
    RETURNS TRIGGER AS $$
BEGIN
    NEW.increment = NEXTVAL(TG_ARGV[0]);
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS trigger_update ON tidy.users CASCADE;
CREATE TRIGGER trigger_update BEFORE UPDATE ON tidy.users
    FOR EACH ROW EXECUTE PROCEDURE set_update('tidy.users_increment_seq');

DROP TRIGGER IF EXISTS trigger_update ON tidy.user_tokens CASCADE;
CREATE TRIGGER trigger_update BEFORE UPDATE ON tidy.user_tokens
    FOR EACH ROW EXECUTE PROCEDURE set_update('tidy.user_tokens_increment_seq');

CREATE EXTENSION IF NOT EXISTS pg_trgm;

DROP TYPE IF EXISTS tidy.issue_action CASCADE;
CREATE TYPE tidy.issue_action AS ENUM (
    'view',
    'create',
    'edit_own',
    'edit_any',
    'delete_own',
    'delete_any'
);

CREATE TABLE IF NOT EXISTS tidy.queues (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    key TEXT NOT NULL,
    name TEXT NOT NULL,
    owner_id UUID NOT NULL
        REFERENCES tidy.users(id) ON DELETE RESTRICT,
    removed BOOLEAN NOT NULL DEFAULT false,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    increment BIGSERIAL UNIQUE NOT NULL,

    CONSTRAINT queues_unique_key UNIQUE (key)
);

CREATE INDEX IF NOT EXISTS idx_queues_owner_id_removed
    ON tidy.queues (owner_id, removed);

CREATE INDEX IF NOT EXISTS idx_queues_key_trgm
    ON tidy.queues USING gin (key gin_trgm_ops);

CREATE INDEX IF NOT EXISTS idx_queues_name_trgm
    ON tidy.queues USING gin (name gin_trgm_ops);

DROP TRIGGER IF EXISTS trigger_update ON tidy.queues CASCADE;
CREATE TRIGGER trigger_update BEFORE UPDATE ON tidy.queues
    FOR EACH ROW EXECUTE PROCEDURE set_update('tidy.queues_increment_seq');

CREATE TABLE IF NOT EXISTS tidy.queue_role_permissions (
    queue_id UUID NOT NULL
        REFERENCES tidy.queues(id) ON DELETE RESTRICT,
    role tidy.user_role NOT NULL,
    actions tidy.issue_action[] NOT NULL,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    PRIMARY KEY (queue_id, role)
);

CREATE INDEX IF NOT EXISTS idx_queue_role_permissions_actions_gin
    ON tidy.queue_role_permissions USING gin (actions);

CREATE TABLE IF NOT EXISTS tidy.queue_user_permissions (
    queue_id UUID NOT NULL
        REFERENCES tidy.queues(id) ON DELETE RESTRICT,
    user_id UUID NOT NULL
        REFERENCES tidy.users(id) ON DELETE RESTRICT,
    actions tidy.issue_action[] NOT NULL,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    PRIMARY KEY (queue_id, user_id)
);

CREATE INDEX IF NOT EXISTS id_queue_user_permissions_actions_gin
    ON tidy.queue_user_permissions USING gin (actions);

DROP TYPE IF EXISTS tidy.issue_status CASCADE;
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

DROP TYPE IF EXISTS tidy.issue_priority CASCADE;
CREATE TYPE tidy.issue_priority AS ENUM (
    'minor',
    'normal',
    'critical',
    'blocker'
);

DROP TYPE IF EXISTS tidy.issue_type CASCADE;
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
    description TEXT NOT NULL,
    type tidy.issue_type NOT NULL DEFAULT 'task',
    status tidy.issue_status NOT NULL DEFAULT 'description',
    priority tidy.issue_priority NOT NULL DEFAULT 'normal',

    component TEXT CHECK (component ~ '^[a-z0-9_-]+$'),
    story_points INTEGER CHECK (story_points > 0),

    author_id UUID NOT NULL
        REFERENCES tidy.users(id) ON DELETE RESTRICT,
    assignee_id UUID
        REFERENCES tidy.users(id) ON DELETE RESTRICT,

    removed BOOLEAN NOT NULL DEFAULT false,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    UNIQUE (queue_id, number)
);

DROP TYPE IF EXISTS tidy.issue_link_type CASCADE;
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

CREATE TABLE IF NOT EXISTS tidy.boards (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    name TEXT NOT NULL,
    queue_ids TEXT[] NOT NULL,
    filters JSONB NOT NULL,

    owner_id UUID NOT NULL
        REFERENCES tidy.users(id) ON DELETE RESTRICT,
    private BOOLEAN NOT NULL DEFAULT true,

    removed BOOLEAN NOT NULL DEFAULT false,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    increment BIGSERIAL UNIQUE NOT NULL
);

DROP TRIGGER IF EXISTS trigger_update ON tidy.boards CASCADE;
CREATE TRIGGER trigger_update BEFORE UPDATE ON tidy.boards
    FOR EACH ROW EXECUTE PROCEDURE set_update('tidy.boards_increment_seq');

