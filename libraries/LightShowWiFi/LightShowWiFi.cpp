
#include "LightShowWiFi.h"
#include "Arduino.h"
#include "ESP8266WiFi.h"

LightShowWiFi::LightShowWiFi(const String AP, const String PASSWD, char *clientName) {
  _AP = AP;
  _PASSWD = PASSWD;
  _clientName = clientName;
}

void LightShowWiFi::connect() {
  WiFi.disconnect();
  WiFi.hostname(_clientName);
  WiFi.begin(_AP, _PASSWD);
  if (_stationDisconnectedHandler != NULL) {
    WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected &event) {
      _stationDisconnectedHandler();
    });
  }

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

void LightShowWiFi::onDisconnect(DisconnectHandler disconnectHandler) {
  _stationDisconnectedHandler = disconnectHandler;
}
