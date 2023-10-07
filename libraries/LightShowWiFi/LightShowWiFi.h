#ifndef LightShowWiFi_h
#define LightShowWiFi_h

#include "Arduino.h"

class LightShowWiFi {
private:
  String _AP;
  String _PASSWD;

public:
  LightShowWiFi(const String AP, const String PASSWD);
  void connect();
  String getIP();
};

#endif
