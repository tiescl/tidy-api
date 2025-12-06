#!/usr/bin/env bash

url="https://api.tidyapp.uz"
route="/admin/v1/users"

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
        "user_id": "05224d05-a680-4c03-a3a1-b894e3758857"
    }' \
)

echo "$res" | sed '/^{/,$d' | sed -n p
echo "$res" | sed -n '/^{/,$p' | jq .
