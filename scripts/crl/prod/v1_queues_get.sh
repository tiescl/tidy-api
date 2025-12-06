#!/usr/bin/env bash

url="https://api.tidyapp.uz"
route="/v1/queues"

res=$(
    curl -s --location "$url$route?limit=5" \
    --include \
    --verbose \
    --request GET \
    --header 'Content-Type: application/json' \
    --header 'Cache-Control: no-cache' \
    --header 'Accept-Language: en' \
    --cookie 'cookie.txt' \
    --cookie-jar 'cookie.txt' \
)

echo "$res" | sed '/^{/,$d' | sed -n p
echo "$res" | sed -n '/^{/,$p' | jq .
