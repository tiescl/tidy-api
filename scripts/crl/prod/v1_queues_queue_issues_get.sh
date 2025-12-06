#!/usr/bin/env bash

url="https://api.tidyapp.uz"
route="/v1/queues/987277d4-23e7-4793-b739-4e861f963f38/issues"

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
)

echo "$res" | sed '/^{/,$d' | sed -n p
echo "$res" | sed -n '/^{/,$p' | jq .
