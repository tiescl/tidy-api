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
    'admin',
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
    '0cbbdb18-6760-4cf9-a653-a6cef58b4d7b',
    'asdf@gmail.com',
    'uzbek_og''loni',
    'abcd',
    'developer',
    FALSE,
    '2025-11-22T13:00:00+00'
), (
    '605223cd-826a-46a7-9398-b21f1dd4fd45',
    'slayer_of_demons',
    'asdf@gmail.com',
    'abcd',
    'pending',
    FALSE,
    '2025-11-22T12:00:00+00'
);
