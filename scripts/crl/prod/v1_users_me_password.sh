#!/usr/bin/env bash

url="https://api.tidyapp.uz"
route="/v1/users/me/password"

res=$(
    curl -s --location "$url$route" \
    --include \
    --verbose \
    --request PUT \
    --header 'Content-Type: application/json' \
    --header 'Cache-Control: no-cache' \
    --header 'Accept-Language: en' \
    --cookie 'cookie.txt' \
    --cookie-jar 'cookie.txt' \
    --data-raw '{
        "old_password": "12345678",
        "new_password": "12345678910"
    }'
)

echo "$res" | sed '/^{/,$d' | sed -n p
echo "$res" | sed -n '/^{/,$p' | jq .
