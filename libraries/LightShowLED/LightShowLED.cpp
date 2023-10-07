#include "Arduino.h"
#include "LightShowLED.h"
#include "FastLED.h"

struct ColorMapping {
  char* name;
  int hue;
};

ColorMapping colorMap[] = {
  { "red", 0 },
  { "orange", 32 },
  { "yellow", 64 },
  { "green", 96 },
  { "blue", 160 },
  { "purple", 192 },
  { "pink", 224 },
  { "white", 0 },  // Note: Hue for white can be any value (it doesn't affect the result)
  { "off", -1 }    // Special case for turning off LEDs
};

const int numColors = sizeof(colorMap) / sizeof(colorMap[0]);

int getHueForColor(char* colorName) {
  for (int i = 0; i < numColors; i++) {
    if (strcmp(colorName, colorMap[i].name) == 0) {
      return colorMap[i].hue;
    }
  }
  return -1;
};

LightShowLED::LightShowLED(int numLEDs) {
  _colorIndex = 0;
  _numLEDs = numLEDs;
  _leds = new CRGB[numLEDs];
  _maxFrameRate = 16;
}

void LightShowLED::setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, RGB>(_leds, _numLEDs)
    .setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(255);
  FastLED.clear(true);
}

void LightShowLED::fillColor(char* color) {
  int hue = getHueForColor(color);

  if (hue == -1) {
    fill_solid(_leds, _numLEDs, CRGB::Black);
  } else {
    fill_solid(_leds, _numLEDs, CHSV(hue, 255, 255));
  }
  FastLED.delay(_maxFrameRate);
}

void LightShowLED::fade(int time, char* color, bool up) {
  const int hue = getHueForColor(color);
  unsigned long frameRate = time / 250;
  double factor = 1;
  if (frameRate < _maxFrameRate) {
    factor = (double)(_maxFrameRate + 2) / (double)frameRate;
    frameRate = _maxFrameRate;
  }

  double i = up ? 5 : 255;

  long start = millis();
  if (up) {
    while (i < 255) {
      fill_solid(_leds, _numLEDs, CHSV(hue, 255, ceil(i)));
      FastLED.delay(frameRate);
      i += factor;
    }
  } else {
    while (i > 5) {
      fill_solid(_leds, _numLEDs, CHSV(hue, 255, i));
      FastLED.delay(frameRate);
      i -= factor;
    }
  }
  long duration = millis() - start;
  // Serial.printf("time %d, hue %d, frameRate %d\n", time, hue, frameRate);
  // Serial.printf("factor %f, i %f\n", factor, i);
  // Serial.printf("duration %d\n", duration);
}

void LightShowLED::fadeUp(int time, char* color) {
  fade(time, color, true);
}

void LightShowLED::fadeDown(int time, char* color) {
  fade(time, color, false);
}


void LightShowLED::wave(int time, bool up, char* color = NULL, int fadeBy = 50) {
  int newTime = time >= 1000 ? time - ((time / 1000) * 100) : time;
  unsigned long frameRate = newTime / _numLEDs;

  double factor = 1;
  if (frameRate < _maxFrameRate) {
    factor = (double)(_maxFrameRate + 2) / (double)frameRate;
    frameRate = _maxFrameRate;
  } else {
    frameRate -= 2;
  }

  double i = up ? 0 : _numLEDs - 1;
  const long start = millis();
  long duration = 0;

  if (up) {
    while (i < _numLEDs && duration < newTime) {
      for (int j = floor(i); j < floor(i + factor); j++) {
        char* nextColor = color != NULL ? color : getNextColor();
        Serial.printf("nextColor: %s\n", nextColor);
        _leds[j] = CHSV(getHueForColor(nextColor), 255, 255);
      }
      fadeToBlackBy(_leds, _numLEDs, fadeBy);
      FastLED.delay(frameRate);
      i += factor;
      duration = millis() - start;
    }
  } else {
    while (i > 0 && duration < newTime) {
      for (int j = ceil(i); j > ceil(i - factor); j--) {
        char* nextColor = color != NULL ? color : getNextColor();
        _leds[j] = CHSV(getHueForColor(nextColor), 255, 255);
      }
      fadeToBlackBy(_leds, _numLEDs, fadeBy);
      FastLED.delay(frameRate);
      i -= factor;
      duration = millis() - start;
    }
  }

  duration = millis() - start;
  // Serial.printf("duration %d\n", duration);
}

void LightShowLED::waveUp(int time, char* color = NULL) {
  wave(time, true, color);
}

void LightShowLED::waveDown(int time, char* color = NULL) {
  wave(time, false, color);
}

void LightShowLED::waveUp(int time) {
  wave(time, true, NULL);
}

void LightShowLED::waveDown(int time) {
  wave(time, false, NULL);
}

void LightShowLED::off() {
  fill_solid(_leds, _numLEDs, CRGB::Black);
  FastLED.delay(_maxFrameRate);
}

void LightShowLED::setColors(char** colors, int size) {
  _colors = colors;
  _colorsLength = size;
  Serial.printf("colorsLength: %d\n", _colorsLength);
}

char* LightShowLED::getNextColor() {
  _lastColor = _colors[_colorIndex];
  Serial.printf("colorIndex: %d, color: %s\n", _colorIndex, _lastColor);
  _colorIndex = (_colorIndex + 1) % _colorsLength;
  return _lastColor;
}


char* LightShowLED::getLastColor() {
  return _lastColor;
}

void LightShowLED::sinWave(int time, bool up, char* color = NULL) {
  int newTime = time >= 1000 ? time - ((time / 1000) * 100) : time;
  unsigned long frameRate = _maxFrameRate;
  int bpm = 60 * 1000 / (time / 4);

  Serial.printf("bpm: %d, newTime: %d\n", bpm, newTime);

  int i = 0;
  const long start = millis();
  long duration = 0;

  while (duration < newTime) {
    char* nextColor = color != NULL ? color : getNextColor();
    uint8_t sinBeat = beatsin16(bpm, 0, _numLEDs - 1, 0, 0);
    _leds[sinBeat] = CHSV(getHueForColor(nextColor), 255, 255);

    fadeToBlackBy(_leds, _numLEDs, 10);
    FastLED.delay(frameRate);
    duration = millis() - start;
  }
}

void LightShowLED::popFade(int time, char* color = NULL) {
  const long start = millis();
  long duration = 0;
  int i = 0;

  char* nextColor = color != NULL ? color : getNextColor();
  fill_solid(_leds, _numLEDs, CHSV(getHueForColor(nextColor), 255, 255));
  FastLED.delay(time / 4);

  int fadeBy = 50;

  while (duration < time) {
    fadeToBlackBy(_leds, _numLEDs, fadeBy);
    FastLED.delay(_maxFrameRate);
    fadeBy -= 1;
    duration = millis() - start;
  }
  // Serial.printf("duration %d\n", duration);
}
