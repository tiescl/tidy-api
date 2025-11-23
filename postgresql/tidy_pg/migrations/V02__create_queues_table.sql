CREATE EXTENSION IF NOT EXISTS pg_trgm;

DROP TYPE IF EXISTS tidy.issue_action;
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

DROP TRIGGER IF EXISTS trigger_update ON tidy.queues;
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
