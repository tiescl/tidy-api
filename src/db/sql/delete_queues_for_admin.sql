-- $1 - queue_ids

UPDATE tidy.queues
SET removed = TRUE
WHERE id = ANY($1);
