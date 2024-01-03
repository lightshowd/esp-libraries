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
  int _sectionSize;
  uint _maxFrameRate;
  char** _colors;
  int _colorsLength;
  char* _lastColor;
  CRGB* _leds;
  EOrder _colorOrder;

public:
  LightShowLED(int numLEDs, int brightness = 120, EOrder colorOrder = RGB);
  void setup();
  void setFrameRate(uint frameRate);
  void setSectionSize(int sectionSize);
  void fillColor(char* color = NULL);
  void rotateFill(int index = 0);
  void fadeUp(int time, char* color);
  void fadeDown(int time, char* color);
  void fade(int time, char* color, bool up);
  void waveUp(int time);
  void waveUp(int time, char* color);
  void waveDown(int time);
  void waveDown(int time, char* color);
  void waveUpDown(int time, char* color);
  void wave(int time, bool up, char* color, int fadeBy, bool doubleWave);
  void sinWave(int time, bool up, char* color);
  void popFade(int time, char* color = NULL, uint8_t rotateIndex = NULL);
  void sparkles(int time, char* color = NULL);
  void setColors(char** colors, int size);
  void off();
  char* getNextColor(int index = NULL);
  char* getLastColor();
};

#endif
