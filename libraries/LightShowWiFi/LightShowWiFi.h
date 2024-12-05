#ifndef LightShowWiFi_h
#define LightShowWiFi_h

#include "Arduino.h"

class LightShowWiFi {
  typedef std::function<void()> DisconnectHandler;
private:
  String _AP;
  String _PASSWD;
  char *_clientName;
  DisconnectHandler _stationDisconnectedHandler;

public:
  LightShowWiFi(const String AP, const String PASSWD, char *clientName);
  void connect();
  String getIP();
  void onDisconnect(DisconnectHandler disconnectHandler);
};

#endif
