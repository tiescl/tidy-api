INSERT INTO tidy.queues (
    id,
    key,
    name,
    owner_id,
    created_at,
    updated_at,
    removed
)
VALUES (
    '5d854c28-c6eb-4ed4-b429-aaf006cea6b5',
    'TIDYBACK',
    'Queue for tracking backend related tickets for tidy',
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    '2025-10-10T00:00:00+00',
    '2025-10-10T00:00:00+00',
    FALSE
), (
    '7d854c28-c6eb-4ed4-b429-aaf006cea6b5',
    'TRASH',
    'Queue for useless tickets (purged)',
    'a9f70642-6e70-4c86-97ff-455d659bede0',
    '2025-10-10T08:00:00+00',
    '2025-10-10T08:00:00+00',
    TRUE
);
