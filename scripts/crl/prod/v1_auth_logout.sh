#!/usr/bin/env bash

url="https://api.tidyapp.uz"
route="/v1/auth/logout"

res=$(
    curl -s --location "$url$route" \
    --include \
    --verbose \
    --request POST \
    --header 'Content-Type: application/json' \
    --header 'Cache-Control: no-cache' \
    --header 'Accept-Language: en' \
    --cookie 'cookie.txt' \
    --cookie-jar 'cookie.txt'
)

echo "$res" | sed '/^{/,$d' | sed -n p
echo "$res" | sed -n '/^{/,$p' | jq .
