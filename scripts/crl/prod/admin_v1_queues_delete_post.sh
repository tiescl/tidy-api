#!/usr/bin/env bash

url="https://api.tidyapp.uz"
route="/admin/v1/queues/delete"

res=$(
    curl -s --location "$url$route" \
    --include \
    --verbose \
    --request POST \
    --header 'Content-Type: application/json' \
    --header 'Cache-Control: no-cache' \
    --header 'Accept-Language: en' \
    --cookie 'cookie.txt' \
    --cookie-jar 'cookie.txt' \
    --data-raw '{
        "queue_ids": ["0c695c85-5cb2-4df4-9ec7-d2a47f9144e4"]
    }' \
)

echo "$res" | sed '/^{/,$d' | sed -n p
echo "$res" | sed -n '/^{/,$p' | jq .
