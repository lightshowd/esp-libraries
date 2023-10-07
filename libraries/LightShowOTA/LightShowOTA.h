#ifndef LightShowOTA_h
#define LightShowOTA_h

#include "Arduino.h"

class LightShowOTA {
private:
  char *_clientName;

public:
  LightShowOTA(char *clientName);
  void initialize();
  void listen();
};

#endif