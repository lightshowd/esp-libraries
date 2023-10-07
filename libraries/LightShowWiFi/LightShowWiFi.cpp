
#include "LightShowWiFi.h"
#include "Arduino.h"
#include "ESP8266WiFi.h"

LightShowWiFi::LightShowWiFi(const String AP, const String PASSWD) {
  _AP = AP;
  _PASSWD = PASSWD;
}

void LightShowWiFi::connect() {
  WiFi.disconnect();
  WiFi.begin(_AP, _PASSWD);

  // WiFi.disconnect();
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.printf("Connecting");
  }

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  String ip = WiFi.localIP().toString();
  Serial.printf("[SETUP] WiFi Connected %s\n", ip.c_str());
}

String LightShowWiFi::getIP() {
  return WiFi.localIP().toString();
}
