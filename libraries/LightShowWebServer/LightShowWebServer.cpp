
#include "LightShowWebServer.h"
#include "Arduino.h"
#include "ESP8266mDNS.h"
#include "ESP8266WebServer.h"


void LightShowWebServer::bindListener(TestHandler handler) {
  server.on("/", [handler, this]() {
    int time = server.arg("time").toInt();
    const char* color = server.arg("color").c_str();
    const char* effect = server.arg("effect").c_str();
    char* passColor = strcmp(color, "null") == 0 ? (char*)NULL : (char*)color;
    handler(time, passColor, effect);
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
