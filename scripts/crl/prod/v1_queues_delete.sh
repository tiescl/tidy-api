#!/usr/bin/env bash

url="https://api.tidyapp.uz"
route="/v1/queues"

res=$(
    curl -s --location "$url$route" \
    --include \
    --verbose \
    --request DELETE \
    --header 'Content-Type: application/json' \
    --header 'Cache-Control: no-cache' \
    --header 'Accept-Language: en' \
    --cookie 'cookie.txt' \
    --cookie-jar 'cookie.txt' \
    --data-raw '{
        "queue_id": "2afc46fb-379b-4ba0-93a5-75b8d935208a"
    }' \
)

echo "$res" | sed '/^{/,$d' | sed -n p
echo "$res" | sed -n '/^{/,$p' | jq .
