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
        "key": "TIDYTEST",
        "name": "Just a test queue",
        "owner_id": "ca003bb9-dcce-4b87-98dc-4969fed92354"
    }' \
)

echo "$res" | sed '/^{/,$d' | sed -n p
echo "$res" | sed -n '/^{/,$p' | jq .
