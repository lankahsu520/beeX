# Overview
Beex is an open source message handler that implements the MCTT protocol.

What is MCTT
1. It is base on multicast (udp) protocol.
2. It is similar to MQTT, but it is lightweight.
3. There's no need to build a message broker on lan.
4. Each daemon (application) can be a server or client.
5. Fixed Data Struct and include zwave (hcapi v8_12)

# Depend on
- [utilx9](https://github.com/lankahsu520/utilx9)
- [zwave/zware][https://www.silabs.com/wireless/z-wave]
- [github_](https://github.com/lankahsu520/github_)

# Current Status



# Build
   ```
$ make
   ```

# Example or Usage
- beex_123 - an example. automatically trigger tamper notification and receive the notification.


# License
beex is under the New BSD License (BSD-3-Clause).


# Documentation
Run an example and read it.
