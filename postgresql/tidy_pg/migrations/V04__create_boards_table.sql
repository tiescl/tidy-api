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
