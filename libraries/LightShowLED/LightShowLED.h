#ifndef LightShowLED_h
#define LightShowLED_h

#include "Arduino.h"

#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER

#include "FastLED.h"

#define LED_TYPE WS2811
#define DATA_PIN 1

enum class PaletteType {
  XMAS,
  HALLOWEEN,
  USA
};

enum class LEDEffectVelocity {
  POP_FADE = 8,     // 64
  FADE_OFF = 9,     // 72
  SIN_WAVE = 10,    // 80
  FADE_UP = 11,     // 88
  SOLID = 12,       // 96
  WAVE_UP = 14,     // 112
  FADE_DOWN = 1,    // 1
  WAVE_DOWN = 2,    // 16
  DOUBLE_WAVE = 4,  // 32
  SPARKLES = 6      // 48
};



class LightShowLED {
private:
  int _brightness;
  int _numLEDs;
  int _colorIndex;
  int _sectionSize;
  int _multiColors;
  uint _maxFrameRate;
  char** _colors;
  int _colorsLength;
  char* _lastColor;
  CRGB* _leds;
  EOrder _colorOrder;
  uint8_t _standByPaletteIndex = 0;
  bool _standByPaletteOn;
  uint8_t _gHue = 0;
  CRGBPalette16 _currentPalette;
  PaletteType _paletteType;
  CRGBPalette16* _paletteTheme;
  uint8_t _paletteThemeLength;


public:
  LightShowLED(int numLEDs, int brightness = 100, EOrder colorOrder = RGB);
  void setup();
  void setFrameRate(uint frameRate);
  void setSectionSize(int sectionSize);
  void fillColor(char* color = NULL);
  void fillColorDuration(int time, char* color = NULL);
  void rotateFill(int index = 0);
  void treeFill(int index = 0);
  void fadeUp(int time, char* color);
  void fadeUpOff(int time, char* color);
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
  void setColors(char** colors, int size, int multiColors);
  void off();
  char* getNextColor(int index = NULL);
  char* getLastColor();
  int playVelocityEffect(int velocity, int length, char* color, int noteIndex);
  void rainbow();
  void startStandbyPalette(PaletteType paletteType);
  void stopStandByPalette();
  void runStandBy();
  void nextPattern();
};

#endif
