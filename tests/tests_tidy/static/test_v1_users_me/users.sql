INSERT INTO tidy.users (
    id,
    email,
    username,
    password_hash,
    role,
    removed,
    created_at
)
VALUES (
    '774cc050-2542-4c05-9027-f4919112fc81',
    'demon_slayer',
    'qwer@gmail.com',
    'abcd',
    'admin',
    FALSE,
    '2025-11-22T13:00:00+00'
), (
    '5ac15ada-0726-4590-92b8-59e0ef096afa',
    'harmonic_electrode',
    'electro@gmail.com',
    'abcd',
    'pending',
    TRUE,
    '2025-11-22T10:00:00+00'
);
