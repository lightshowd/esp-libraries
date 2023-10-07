#ifndef LightShowLED_h
#define LightShowLED_h

#include "Arduino.h"

#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER

#include "FastLED.h"

#define LED_TYPE WS2811
#define DATA_PIN 1


class LightShowLED {
private:
  int _brightness;
  int _numLEDs;
  int _colorIndex;
  uint _maxFrameRate;
  char** _colors;
  int _colorsLength;
  char* _lastColor;
  CRGB* _leds;

public:
  LightShowLED(int numLEDs);
  void setup();
  void fillColor(char* color);
  void fadeUp(int time, char* color);
  void fadeDown(int time, char* color);
  void fade(int time, char* color, bool up);
  void waveUp(int time);
  void waveUp(int time, char* color);
  void waveDown(int time);
  void waveDown(int time, char* color);
  void wave(int time, bool up, char* color, int fadeBy);
  void sinWave(int time, bool up, char* color);
  void popFade(int time, char* color);
  void setColors(char** colors, int size);
  void off();
  char* getNextColor();
  char* getLastColor();
};

#endif
