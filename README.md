# Serial-logging to Syslog
Guess you're running a microcontroller (1), that restarts every now and then and you want to debug know. As leaving it connected to a computer is (sometimes) not desirable, I wrote this sketch for ESP8266. So ESP8266 (2) needs to be serially connected to your microcontroller and needs to be part of your WiFi.

## Setup
* Install rsyslogd (maybe on a Raspberry Pi)
* Edit /etc/rsyslog.conf and add <br />
`module(load="imudp")` <br />
input(type="imudp" port="514") <br />
* Connect TX of (1) with RX of (2), connect RX of (1) with TX of (2), connect GND of (1) to GND of (2)
* Edit port, IP and WiFi-credentials in `main.cpp` according your needs
* That's all. After restart of (2) messages should appear (depending on your config) most likely in /var/log/syslog.