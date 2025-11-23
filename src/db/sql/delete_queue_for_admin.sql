-- $1 - queue_id

UPDATE tidy.queues
SET removed = TRUE
WHERE id = $1;
