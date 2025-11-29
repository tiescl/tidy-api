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
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    'noname@gmail.com',
    'whoami',
    'abcd',
    'pending',
    FALSE,
    '2025-11-22T13:00:00+00'
);
