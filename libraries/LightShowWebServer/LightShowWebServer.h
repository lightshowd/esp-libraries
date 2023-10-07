#ifndef LightShowWebServer_h
#define LightShowWebServer_h

#include "Arduino.h"
#include "ESP8266WebServer.h"

class LightShowWebServer {
  typedef std::function<void(int time, const char* color)> TestHandler;

private:
  ESP8266WebServer server;


public:
  LightShowWebServer(int port)
    : server(port) {}


  void bindListener(TestHandler handler);
  void listen();
  void send();
};

#endif
