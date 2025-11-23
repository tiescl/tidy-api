CREATE SCHEMA IF NOT EXISTS tidy;

DROP TYPE IF EXISTS tidy.user_role;
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

DROP TRIGGER IF EXISTS trigger_update ON tidy.users;
CREATE TRIGGER trigger_update BEFORE UPDATE ON tidy.users
    FOR EACH ROW EXECUTE PROCEDURE set_update('tidy.users_increment_seq');

DROP TRIGGER IF EXISTS trigger_update ON tidy.user_tokens;
CREATE TRIGGER trigger_update BEFORE UPDATE ON tidy.user_tokens
    FOR EACH ROW EXECUTE PROCEDURE set_update('tidy.user_tokens_increment_seq');
