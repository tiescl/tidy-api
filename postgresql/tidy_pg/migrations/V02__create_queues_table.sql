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
    key TEXT NOT NULL UNIQUE,
    name TEXT NOT NULL,
    owner_id UUID NOT NULL
        REFERENCES tidy.users(id) ON DELETE RESTRICT,
    removed BOOLEAN NOT NULL DEFAULT false,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    increment BIGSERIAL UNIQUE NOT NULL
);

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

CREATE TABLE IF NOT EXISTS tidy.queue_user_permissions (
    queue_id UUID NOT NULL
        REFERENCES tidy.queues(id) ON DELETE RESTRICT,
    user_id UUID NOT NULL
        REFERENCES tidy.users(id) ON DELETE RESTRICT,
    actions tidy.issue_action[] NOT NULL,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    PRIMARY KEY (queue_id, user_id)
);
