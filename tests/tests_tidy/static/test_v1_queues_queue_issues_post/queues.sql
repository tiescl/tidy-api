INSERT INTO tidy.queues (
    id,
    key,
    name,
    owner_id,
    removed
)
VALUES (
    '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
    'TIDYBACK',
    'Queue for tracking backend related tickets for tidy',
    '774cc050-2542-4c05-9027-f4919112fc81',
    FALSE
), (
    '5bf11df6-3c74-4962-a272-aed7c9e35f22',
    'TIDYFRONT',
    'Queue for tracking frontend related tickets for tidy',
    '5ac15ada-0726-4590-92b8-59e0ef096afa',
    FALSE
), (
    '8cb68c39-1cd0-4a57-960d-c13f3b5c5e4a',
    'TIDYDUTY',
    'Queue for tracking tickets from bughunters',
    '5ac15ada-0726-4590-92b8-59e0ef096afa',
    FALSE
), (
    '57996a05-fb8c-454d-910e-8b53d313741e',
    'TIDYARCHREVIEW',
    'Queue for tracking tickets for architecture reviews',
    '5ac15ada-0726-4590-92b8-59e0ef096afa',
    FALSE
);

INSERT INTO tidy.queue_user_permissions (
    queue_id,
    user_id,
    actions
)
VALUES (
    '8cb68c39-1cd0-4a57-960d-c13f3b5c5e4a',
    '774cc050-2542-4c05-9027-f4919112fc81',
    '{"create"}'
);

INSERT INTO tidy.queue_role_permissions (
    queue_id,
    role,
    actions
)
VALUES (
    '57996a05-fb8c-454d-910e-8b53d313741e',
    'developer',
    '{"create", "delete_own"}'
);

INSERT INTO tidy.issues (
    author_id,
    queue_id,
    number,
    title
)
VALUES (
    '774cc050-2542-4c05-9027-f4919112fc81',
    '8cb68c39-1cd0-4a57-960d-c13f3b5c5e4a',
    1,
    'first ever issue in TIDYDUTY'
);
