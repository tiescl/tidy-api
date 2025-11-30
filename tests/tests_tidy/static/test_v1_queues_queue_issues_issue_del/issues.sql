INSERT INTO tidy.issues (
    id,
    queue_id,
    number,
    title,
    author_id,
    removed,
    created_at,
    updated_at
)
VALUES (
    '264c7fc0-6020-48ae-b10c-d0d1479d9355',
    '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
    1,
    'Some backend task',
    '774cc050-2542-4c05-9027-f4919112fc81',
    FALSE,
    '2025-11-30T00:00:00+00',
    '2025-11-30T00:00:00+00'
), (
    'fb18e7f4-2cfd-4cf8-bbf0-ab575f2a1e6c',
    '1e6fd243-6239-4d76-8f0d-2bcdbc194f18',
    3,
    'Some tidy top task',
    '774cc050-2542-4c05-9027-f4919112fc81',
    FALSE,
    '2025-11-30T00:00:00+00',
    '2025-11-30T00:00:00+00'
), (
    'ed77ea7c-99b0-47c7-8f8d-936d2aced769',
    '1e6fd243-6239-4d76-8f0d-2bcdbc194f18',
    4,
    'Another tidy top task',
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    FALSE,
    '2025-11-30T00:00:00+00',
    '2025-11-30T00:00:00+00'
), (
    '076a4461-5faf-41c3-b061-de510fc2d409',
    '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
    2,
    'Another backend task',
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    TRUE,
    '2025-11-30T00:00:00+00',
    '2025-11-30T00:00:00+00'
), (
    '5fe28b12-824e-4c0a-aee4-f98e9b9aacbc',
    '5bf11df6-3c74-4962-a272-aed7c9e35f22',
    1,
    'Some frontend task',
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    FALSE,
    '2025-11-30T00:00:00+00',
    '2025-11-30T00:00:00+00'
), (
    '4dbef3e1-011a-4536-9473-42b414e1bf79',
    '57996a05-fb8c-454d-910e-8b53d313741e',
    1,
    'Some architecture review task',
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    FALSE,
    '2025-11-30T00:00:00+00',
    '2025-11-30T00:00:00+00'
);
