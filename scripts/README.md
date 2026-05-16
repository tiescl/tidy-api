1. install postgres, then:
```bash
sudo systemctl start postgresql
sudo systemctl enable postgresql
```

postgres logs are managed by journalcrl, to view them:
```bash
sudo journalctl -u postgresql -f
```

2. enable and start custom logrotate service for tidy-api
```bash
sudo systemctl enable tidy-api-logrotate.service
sudo systemctl start tidy-api-logrotate.service
```

3. enable and start custom logrotate timer (every 10min) for tidy-api
```bash
sudo systemctl enable tidy-api-logrotate.timer
sudo systemctl start tidy-api-logrotate.timer
```

4. enable and start the app
```bash
sudo systemctl enable tidy-api.service
sudo systemctl start tidy-api.service
```

----------------------------------------

## Fluent Bit

- Provided config is used with yc-logging plugin to send logs to Yandex.Cloud
- The iam_key is generated with yc cli
- The db for offsets was also created by me (do not forget permissions)
- I did not change the generated systemd file, that one went unchanged

Other stuff about installation and other config is described in:
> https://yandex.cloud/en/docs/logging/tutorials/vm-fluent-bit-logging

----------------------------------------

## nginx

- For nginx, the other files are all default (including systemd)
- Got the SSL cert using Let's Encrypt via Yandex Cert Manager
- Configured an A record in DNS to point api subdomain to server's IP
