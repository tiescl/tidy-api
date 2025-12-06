INSERT INTO tidy.issues (
    id,
    queue_id,
    number,
    title,
    author_id,
    assignee_id,
    removed,
    created_at,
    updated_at
)
VALUES (
    '264c7fc0-6020-48ae-b10c-d0d1479d9355',
    '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
    1,
    'Task 1 for bugfixing',
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    '774cc050-2542-4c05-9027-f4919112fc81',
    FALSE,
    '2025-11-30T01:00:00+00',
    '2025-11-30T01:00:00+00'
), (
    '5fe28b12-824e-4c0a-aee4-f98e9b9aacbc',
    '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
    2,
    'Task 2 for refactoring',
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    NULL,
    FALSE,
    '2025-11-30T02:00:00+00',
    '2025-11-30T02:00:00+00'
), (
    '2d7227a4-ba35-4c42-a24a-34194c07e814',
    '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
    3,
    'Task 3 for bugfixing',
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    NULL,
    TRUE,
    '2025-11-30T03:00:00+00',
    '2025-11-30T03:00:00+00'
);
