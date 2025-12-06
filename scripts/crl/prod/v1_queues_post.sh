#!/usr/bin/env bash

url="https://api.tidyapp.uz"
route="/v1/queues"

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
        "key": "TIDYDUTY",
        "name": "привет",
        "owner_id": "1f34180c-929d-4641-8d67-54dd11d25ec4"
    }' \
)

echo "$res" | sed '/^{/,$d' | sed -n p
echo "$res" | sed -n '/^{/,$p' | jq .
