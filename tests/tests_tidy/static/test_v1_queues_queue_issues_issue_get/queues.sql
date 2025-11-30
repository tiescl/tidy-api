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
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    '2025-10-10T05:00:00+00',
    FALSE
), (
    '57996a05-fb8c-454d-910e-8b53d313741e',
    'TIDYARCHREVIEW',
    'Queue for tracking tickets for architecture reviews',
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    '2025-10-10T04:00:00+00',
    FALSE
), (
    '5bf11df6-3c74-4962-a272-aed7c9e35f22',
    'TIDYFRONT',
    'Queue for tracking frontend related tickets for tidy',
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    '2025-10-10T01:00:00+00',
    TRUE
);

INSERT INTO tidy.queue_user_permissions (
    queue_id,
    user_id,
    actions
)
VALUES (
    '1e6fd243-6239-4d76-8f0d-2bcdbc194f18',
    '774cc050-2542-4c05-9027-f4919112fc81',
    '{"view"}'
);
