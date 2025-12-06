#!/usr/bin/env bash

url="https://api.tidyapp.uz"
route="/v1/queues/344c4f38-08ae-43dc-a289-9cae1f5a97e3/issues/dfa087cb-c7df-4e26-9500-e7f606274a51"

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
        "status": "in_progress"
    }' \
)

echo "$res" | sed '/^{/,$d' | sed -n p
echo "$res" | sed -n '/^{/,$p' | jq .
