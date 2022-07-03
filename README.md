# 1. Overview
Beex is an open source message handler that implements the MCTT protocol.

What is MCTT
1. It is base on multicast (udp) protocol.
2. It is similar to MQTT, but it is lightweight.
3. There's no need to build a message broker on lan.
4. Each daemon (application) can be a server or client.
5. Fixed Data Struct and include zwave (hcapi v8_12)

# 2. Depend on
- [utilx9](https://github.com/lankahsu520/utilx9)
- [zwave/zware](https://www.silabs.com/wireless/z-wave)

# 3. Current Status



# 4. Build
   ```
$ make
   ```

# 5. Example or Usage
- beex_123 - an example. automatically trigger tamper notification and receive the notification.
```bash
# run as server
$ ./beex_123 -d 2 -i enp0s3 &
# this will get bee_notify_tamper !

# run as client
$ ./beex_123 -d 2 -i enp0s3 -c
# this will be a client.
# To trigger-bee_topic_issue_caller every second.
# Also get bee_notify_tamper !

```

# 6. License
beex is under the New BSD License (BSD-3-Clause).


# 7. Documentation
Run an example and read it.
