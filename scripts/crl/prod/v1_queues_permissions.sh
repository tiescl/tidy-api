#!/usr/bin/env bash

url="https://api.tidyapp.uz"
route="/v1/queues/7c3bb664-4947-468c-a11d-ea806910cbb7/permissions"

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
        "permission_type": "role",
        "role": "pending",
        "actions": ["view"]
    }' \
)

echo "$res" | sed '/^{/,$d' | sed -n p
echo "$res" | sed -n '/^{/,$p' | jq .
