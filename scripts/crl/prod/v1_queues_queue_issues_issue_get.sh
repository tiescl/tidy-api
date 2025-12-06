#!/usr/bin/env bash

url="https://api.tidyapp.uz"
route="/v1/queues/344c4f38-08ae-43dc-a289-9cae1f5a97e3/issues/c85b079c-821c-403b-8a9e-eda2944acd4d"

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
