#!/usr/bin/env bash

url="https://api.tidyapp.uz"
route="/v1/queues/987277d4-23e7-4793-b739-4e861f963f38/permissions/delete"

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
        "permission_type": "user",
        "user_id": "91d6241c-02e5-46dd-a49c-0c1bb0348345"
    }' \
)

echo "$res" | sed '/^{/,$d' | sed -n p
echo "$res" | sed -n '/^{/,$p' | jq .
