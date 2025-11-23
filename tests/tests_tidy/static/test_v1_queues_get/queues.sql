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
    '5bf11df6-3c74-4962-a272-aed7c9e35f22',
    'TIDYFRONT',
    'Queue for tracking frontend related tickets for tidy',
    '774cc050-2542-4c05-9027-f4919112fc81',
    '2025-10-10T01:00:00+00',
    FALSE
), (
    '8cb68c39-1cd0-4a57-960d-c13f3b5c5e4a',
    'TIDYDUTY',
    'Queue for tracking tickets from bughunters',
    '774cc050-2542-4c05-9027-f4919112fc81',
    '2025-10-10T02:00:00+00',
    FALSE
), (
    '58550d9c-571e-4f30-8a78-368444d9b6d0',
    'TIDYQA',
    'Queue for tracking tickets from QA testers',
    '774cc050-2542-4c05-9027-f4919112fc81',
    '2025-10-10T03:00:00+00',
    FALSE
), (
    '57996a05-fb8c-454d-910e-8b53d313741e',
    'TIDYARCHREVIEW',
    'Queue for tracking tickets for architecture reviews',
    '774cc050-2542-4c05-9027-f4919112fc81',
    '2025-10-10T04:00:00+00',
    FALSE
), (
    '1e6fd243-6239-4d76-8f0d-2bcdbc194f18',
    'TIDYTOP',
    'Some noname guy''s queue',
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    '2025-10-10T05:00:00+00',
    FALSE
), (
    'c7838595-1516-4d86-a22a-6bac8cc46961',
    'TEST',
    'Another noname guy''s queue',
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    '2025-10-10T06:00:00+00',
    FALSE
), (
    '10a0687f-d967-45c3-b047-72d4fe81ae00',
    'STUDYWITHME',
    'Yet another noname guy''s queue',
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    '2025-10-10T07:00:00+00',
    FALSE
), (
    '7d854c28-c6eb-4ed4-b429-aaf006cea6b5',
    'TRASH',
    'Queue for useless tickets (purged)',
    '774cc050-2542-4c05-9027-f4919112fc81',
    '2025-10-10T08:00:00+00',
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

INSERT INTO tidy.queue_role_permissions (
    queue_id,
    role,
    actions
)
VALUES (
    'c7838595-1516-4d86-a22a-6bac8cc46961',
    'developer',
    '{"view", "create"}'
);
