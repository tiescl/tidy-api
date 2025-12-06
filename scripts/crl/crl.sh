#!/usr/bin/env bash

url="localhost:8080"
route=""

res=$(
    curl -s --location "$url$route" \
    --include \
    --verbose \
    --request GET \
    --header 'Content-Type: application/json' \
    --header 'Cache-Control: no-cache' \
    --header 'Accept-Language: en' \
    --cookie 'cookie.txt' \
    --cookie-jar 'cookie.txt' \
    --data-raw '{
        "hello": "world",
    }' \
)

echo "$res" | sed '/^{/,$d' | sed -n p
echo "$res" | sed -n '/^{/,$p' | jq .
