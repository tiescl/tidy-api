INSERT INTO tidy.queues (
    id,
    key,
    name,
    owner_id,
    created_at,
    removed
)
VALUES (
    '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
    'TIDYBACK',
    'Queue for tracking backend related tickets for tidy',
    '774cc050-2542-4c05-9027-f4919112fc81',
    '2025-10-10T00:00:00+00',
    FALSE
), (
    '1e6fd243-6239-4d76-8f0d-2bcdbc194f18',
    'TIDYTOP',
    'Some noname guy''s queue',
    '774cc050-2542-4c05-9027-f4919112fc81',
    '2025-10-10T05:00:00+00',
    FALSE
), (
    'c7838595-1516-4d86-a22a-6bac8cc46961',
    'TEST',
    'Another noname guy''s queue',
    '774cc050-2542-4c05-9027-f4919112fc81',
    '2025-10-10T06:00:00+00',
    FALSE
);

INSERT INTO tidy.queue_user_permissions (
    queue_id,
    user_id,
    actions
)
VALUES (
    '1e6fd243-6239-4d76-8f0d-2bcdbc194f18',
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    '{"view"}'
);

INSERT INTO tidy.queue_role_permissions (
    queue_id,
    role,
    actions
)
VALUES (
    'c7838595-1516-4d86-a22a-6bac8cc46961',
    'pending',
    '{"view", "create"}'
);
