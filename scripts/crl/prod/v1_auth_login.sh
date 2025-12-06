#!/usr/bin/env bash

url="https://api.tidyapp.uz"
route="/v1/auth/login"

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
        "email": "tiescl.to@gmail.com",
        "password": "12345678910"
    }' \
)
    # "email": "slayer@gmail.com",
    # "password": "12345678"

echo "$res" | sed '/^{/,$d' | sed -n p
echo "$res" | sed -n '/^{/,$p' | jq .
