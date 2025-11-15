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
