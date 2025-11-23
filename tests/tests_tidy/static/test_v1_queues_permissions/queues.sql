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
    '7d854c28-c6eb-4ed4-b429-aaf006cea6b5',
    'TRASH',
    'Queue for useless tickets',
    '774cc050-2542-4c05-9027-f4919112fc81',
    TRUE
);
