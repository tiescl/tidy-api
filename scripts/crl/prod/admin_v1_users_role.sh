#!/usr/bin/env bash

url="https://api.tidyapp.uz"
route="/admin/v1/users/05224d05-a680-4c03-a3a1-b894e3758857/role"

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
        "role": "developer"
    }' \
)

echo "$res" | sed '/^{/,$d' | sed -n p
echo "$res" | sed -n '/^{/,$p' | jq .
