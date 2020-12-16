#include <Arduino.h>
#include <Syslog.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

// Syslog
#define SYSLOG_SERVER "192.168.2.43"                      // IP of Syslog-server
#define SYSLOG_PORT 514                                   // Port of Syslog-server
#define DEVICE_HOSTNAME "ESP8266-Debug"                   // How this client appears at Syslog-server
#define APP_NAME "Serial-Debugger"


const char *ssid = "";                                    // Name of your WiFi
const char *password = "";                                // Password of your WiFi

WiFiUDP wifiUdpClient;

// Syslog
Syslog syslog(wifiUdpClient, SYSLOG_SERVER, SYSLOG_PORT, DEVICE_HOSTNAME, APP_NAME, LOG_KERN);

char strBuf[200];
char newLine[200];
unsigned int lastPointer = 0;
char lastChar;


void pushStringToSyslog(char *buf) {
  syslog.log(LOG_INFO, buf);
}


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  pushStringToSyslog("Serial-Debugger gestartet!");
}


void loop() {
  if (Serial.available() > 0) {
    lastChar = Serial.read();
    Serial.print(lastChar);

    if (lastChar != '\n' && lastChar != '\r') {
      if (lastPointer < (sizeof(strBuf) / sizeof(strBuf[0]) - 1)) {
        *(strBuf + lastPointer++) = lastChar;
      }
    } else if (lastChar == '\n') {
      *(strBuf + lastPointer) = '\0';
      lastPointer = 0;
      strcpy(newLine, strBuf);
      pushStringToSyslog(newLine);
    }
  }
}