
#include "LightShowWebServer.h"
#include "Arduino.h"
#include "ESP8266mDNS.h"
#include "ESP8266WebServer.h"


void LightShowWebServer::bindListener(TestHandler handler) {
  server.on("/", [handler, this]() {
    int time = server.arg("time").toInt();
    const char* color = server.arg("color").c_str();
    handler(time, color);
    this->send();
  });
  server.begin();
}

void LightShowWebServer::listen() {
  server.handleClient();
}

void LightShowWebServer::send() {
  server.send(200, "text/plain", "ok");
}
