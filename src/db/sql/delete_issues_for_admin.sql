-- $1 - issue_ids

UPDATE tidy.issues
SET removed = TRUE
WHERE id = ANY($1);
