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
    'qwer@gmail.com',
    'demon_slayer',
    'abcd',
    'developer',
    FALSE,
    '2025-11-22T13:00:00+00'
), (
    '5ac15ada-0726-4590-92b8-59e0ef096afa',
    'electro@gmail.com',
    'harmonic_electrode',
    'abcd',
    'pending',
    TRUE,
    '2025-11-22T10:00:00+00'
), (
    '5c4b6cc6-10f9-4663-b473-3ffd8cc225f5',
    'potato_mushroom',
    'potato@gmail.com',
    'abcd',
    'manager',
    TRUE,
    '2025-11-22T10:00:00+00'
);
