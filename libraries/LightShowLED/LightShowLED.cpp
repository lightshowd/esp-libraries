#include "Arduino.h"
#include "LightShowLED.h"
#include "LightShowLED_palettes.h"
#include "FastLED.h"

struct ColorMapping {
  char* name;
  CHSV hsv;
};

ColorMapping colorMap[] = {
  { "red", CHSV(0, 255, 255) },
  { "green", CHSV(96, 255, 255) },
  { "purple", CHSV(180, 255, 150) },
  { "blue", CHSV(160, 255, 255) },

  { "gold", CHSV(45, 255, 255) },
  { "white", CHSV(0, 0, 255) },
  { "warmwhite", CHSV(130, 38, 255) },
  { "orange", CHSV(30, 255, 255) },
  { "off", CHSV(0, 0, 0) },
  { "black", CHSV(0, 0, 0) }
};


int findRangeIndex(int value) {
  int range = value >> 3;
  return range;
}

const int numColors = sizeof(colorMap) / sizeof(colorMap[0]);

CHSV getHSV(char* colorName) {
  for (int i = 0; i < numColors; i++) {
    if (strcmp(colorName, colorMap[i].name) == 0) {
      return colorMap[i].hsv;
    }
  }
  return CHSV(0, 0, 0);
};

LightShowLED::LightShowLED(int numLEDs, int brightness, EOrder colorOrder) {
  _colorIndex = 0;
  _numLEDs = numLEDs;
  _leds = new CRGB[numLEDs];
  _brightness = brightness;
  _maxFrameRate = 16;
  _colorOrder = colorOrder;
  _sectionSize = numLEDs / 100 > 0 && numLEDs % 100 == 0 ? 100 : numLEDs;
}

void LightShowLED::setup() {
  if (_colorOrder == RGB) {
    FastLED.addLeds<LED_TYPE, DATA_PIN, RGB>(_leds, _numLEDs);
  }
  if (_colorOrder == GRB) {
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(_leds, _numLEDs);
  }

  FastLED.setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(_brightness);
  FastLED.clear(true);
}

void LightShowLED::setFrameRate(uint frameRate) {
  _maxFrameRate = frameRate;
}

void LightShowLED::setSectionSize(int sectionSize) {
  _sectionSize = sectionSize;
}

void LightShowLED::fillColor(char* color) {
  if (color == NULL) {
    for (int i = 0; i < _numLEDs; i++) {
      _leds[i] = getHSV(getNextColor());
    }
  } else {
    fill_solid(_leds, _numLEDs, getHSV(color));
  }

  FastLED.delay(_maxFrameRate);
}

void LightShowLED::fillColorDuration(int time, char* color) {
  fillColor(color);
  FastLED.delay(time - _maxFrameRate * 3);
  off();
}

void LightShowLED::rotateFill(int index) {
  for (int i = 0; i < _numLEDs; ++i) {
    if (i % _colorsLength == index) {
      _leds[i] = getHSV(getNextColor());
    } else {
      _leds[i] = CRGB::Black;
    }
  }

  FastLED.delay(_maxFrameRate);
}

void LightShowLED::treeFill(int index) {
  for (int i = 0; i < _numLEDs; ++i) {
    if (i % _colorsLength == index) {
      char* nextColor = getNextColor();
      if (nextColor == "green") {
        nextColor = getNextColor();
      }
      _leds[i] = getHSV(nextColor);
    } else {
      _leds[i] = CRGB::Green;
    }
  }

  FastLED.delay(_maxFrameRate);
}

void LightShowLED::fade(int time, char* color, bool up) {
  const CHSV hsvColor = getHSV(color == NULL ? getNextColor() : color);
  unsigned long frameRate = time / hsvColor.v;
  double factor = 1;
  if (frameRate < _maxFrameRate) {
    factor = (double)(_maxFrameRate + 2) / (double)frameRate;
    frameRate = _maxFrameRate;
  }

  if (_numLEDs > 200) {
    factor = factor * 1.5;
  }

  double i = up ? 0 : hsvColor.v;

  long start = millis();

  while (up ? i < hsvColor.v : i > 0) {
    int hsvColorValue = up ? ceil(i) : floor(i);
    if (color != NULL) {
      fill_solid(_leds, _numLEDs, CHSV(hsvColor.hue, hsvColor.sat, hsvColorValue));
    } else {
      for (int j = 0; j < _numLEDs; j++) {
        const CHSV nextColor = getHSV(getNextColor(j));
        _leds[j] = CHSV(nextColor.hue, nextColor.sat, hsvColorValue);
      }
    }

    FastLED.delay(frameRate);
    i = up ? i + factor : i - factor;
  }

  long duration = millis() - start;
  // Serial.printf("time %d, hue %d, frameRate %d\n", time, hue, frameRate);
  // Serial.printf("factor %f, i %f\n", factor, i);
  // Serial.printf("duration %d\n", duration);
}

void LightShowLED::fadeUp(int time, char* color) {
  fade(time, color, true);
}

void LightShowLED::fadeUpOff(int time, char* color) {
  fade(time, color, true);
  off();
}

void LightShowLED::fadeDown(int time, char* color) {
  fade(time, color, false);
}


void LightShowLED::wave(int time, bool up, char* color = NULL, int fadeBy = 51, bool doubleWave = false) {
  int newTime = time >= 1000 ? time - ((time / 1000) * 100) : time;

  unsigned long frameRate = newTime / _numLEDs;



  double factor = 1;
  if (frameRate < _maxFrameRate) {
    factor = (double)(_maxFrameRate + 2) / (double)frameRate;
    frameRate = _maxFrameRate;
  } else {
    frameRate -= 2;
  }

  newTime -= frameRate * 8;

  double i = up ? 0 : _numLEDs - 1;
  const long start = millis();
  long duration = 0;

  if (up) {
    while (i < _numLEDs && duration < newTime) {
      for (int j = floor(i); j < floor(i + factor); j++) {
        char* nextColor = color != NULL ? color : getNextColor();
        CHSV color = getHSV(nextColor);
        _leds[j] = color;
        if (doubleWave) {
          _leds[_numLEDs - j] = color;
        }
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
        CHSV hsvColor = getHSV(nextColor);
        _leds[j] = hsvColor;
      }
      fadeToBlackBy(_leds, _numLEDs, fadeBy);
      FastLED.delay(frameRate);
      i -= factor;
      duration = millis() - start;
    }
  }

  for (int = 0; i < 8; ++i) {
    fadeToBlackBy(_leds, _numLEDs, fadeBy);
    FastLED.delay(frameRate);
  }

  duration = millis() - start;
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

void LightShowLED::waveUpDown(int time, char* color = NULL) {
  wave(time / 2, true, color);
  wave(time / 2, false, color);
}

void LightShowLED::off() {
  fill_solid(_leds, _numLEDs, CRGB::Black);
  FastLED.show();
  FastLED.delay(_maxFrameRate * 2);
}

void LightShowLED::setColors(char** colors, int size, int multiColors) {
  _colors = colors;
  _colorsLength = size;
  _multiColors = multiColors;
  Serial.printf("colorsLength: %d\n", _colorsLength);
}

char* LightShowLED::getNextColor(int index) {
  // If index is passed in, return a consistent color
  if (index != NULL) {
    return _colors[index % _colorsLength];
  }

  if (_multiColors == 0) {
    _lastColor = _colors[_colorIndex];
    _colorIndex = (_colorIndex + 1) % _colorsLength;
    return _lastColor;
  }

  while ((_multiColors & (int)pow(2, _colorIndex)) == 0) {
    _colorIndex = (_colorIndex + 1) % _colorsLength;
  }
  _lastColor = _colors[_colorIndex];
  _colorIndex = (_colorIndex + 1) % _colorsLength;
  return _lastColor;
}


char* LightShowLED::getLastColor() {
  return _lastColor;
}

void LightShowLED::sinWave(int time, bool up, char* color = NULL) {
  int newTime = time * 0.95;
  unsigned long frameRate = _maxFrameRate;
  int bpm = 60 * 1000 / (time / 4);

  // Serial.printf("bpm: %d, newTime: %d\n", bpm, newTime);

  const long start = millis();
  long duration = 0;

  const int ledSections = _numLEDs / _sectionSize;

  while (duration < newTime) {
    char* nextColor = color != NULL ? color : getNextColor();
    CHSV hsvColor = getHSV(nextColor);

    for (int i = 0; i < ledSections; ++i) {
      uint8_t sinBeat = beatsin16(bpm, 0, _sectionSize - 1, 0, 0);
      _leds[sinBeat + (_sectionSize * i)] = hsvColor;
    }

    fadeToBlackBy(_leds, _numLEDs, 10);
    FastLED.delay(frameRate);
    duration = millis() - start;
  }
}

void LightShowLED::sparkles(int time, char* color) {
  int newTime = time;
  unsigned long frameRate = _maxFrameRate;
  int bpm = 60 * 1000 / (time / 4);

  // Serial.printf("bpm: %d, newTime: %d\n", bpm, newTime);
  // Serial.println(color);
  const long start = millis();
  long duration = 0;

  const int ledSections = _numLEDs / _sectionSize;

  while (duration < newTime) {
    char* nextColor = color != NULL ? color : getNextColor();
    CHSV hsvColor = getHSV(nextColor);

    uint8_t rand = random8();

    if (rand < _numLEDs * 1.5) {
      uint8_t sinBeat = beatsin16(bpm * 8, 0, _numLEDs - 1, 0, 0);
      _leds[(sinBeat + rand) % _numLEDs] = hsvColor;
    }

    fadeToBlackBy(_leds, _numLEDs, 10);
    FastLED.delay(frameRate / 4);
    duration = millis() - start;
  }
}


void LightShowLED::popFade(int time, char* color, uint8_t rotateIndex) {
  const long start = millis();
  long duration = 0;
  int i = 0;

  if (color == NULL) {
    for (int j = 0; j < _numLEDs; j++) {
      if (rotateIndex == NULL || (j % _colorsLength) != rotateIndex) {
        _leds[j] = getHSV(getNextColor());
      } else {
        _leds[j] = CRGB::Black;
      }
    }
  } else {
    fill_solid(_leds, _numLEDs, getHSV(color));
  }

  FastLED.delay(time / 4);

  int fadeBy = 50;

  while (duration < time) {
    fadeToBlackBy(_leds, _numLEDs, fadeBy);
    FastLED.delay(_maxFrameRate);
    fadeBy -= 1;
    duration = millis() - start;
  }

  off();
  // Serial.printf("duration %d\n", duration);
}


int LightShowLED::playVelocityEffect(int velocity, int length, char* color, int noteIndex) {
  int rangeIndex = findRangeIndex(velocity);

  if (rangeIndex == static_cast<int>(LEDEffectVelocity::POP_FADE)) {
    popFade(length, color, noteIndex);
    return rangeIndex;
  }

  if (rangeIndex == static_cast<int>(LEDEffectVelocity::SOLID)) {
    // fillColorDuration(length, color);
    fillColor(color);
    return rangeIndex;
  }

  if (rangeIndex == static_cast<int>(LEDEffectVelocity::WAVE_UP)) {
    waveUp(length, color);
    return rangeIndex;
  }

  if (rangeIndex == static_cast<int>(LEDEffectVelocity::WAVE_DOWN)) {
    waveDown(length, color);
    return rangeIndex;
  }

  if (rangeIndex == static_cast<int>(LEDEffectVelocity::SIN_WAVE)) {
    sinWave(length, true, color);
    return rangeIndex;
  }

  if (rangeIndex == static_cast<int>(LEDEffectVelocity::FADE_UP)) {
    fadeUp(length, color);
    return rangeIndex;
  }

  if (rangeIndex == static_cast<int>(LEDEffectVelocity::FADE_OFF)) {
    fadeUpOff(length, color);
    return rangeIndex;
  }

  if (rangeIndex == static_cast<int>(LEDEffectVelocity::FADE_DOWN)) {
    fadeDown(length, color);
    return rangeIndex;
  }

  if (rangeIndex == static_cast<int>(LEDEffectVelocity::SPARKLES)) {
    sparkles(length, color);
    return rangeIndex;
  }

  if (rangeIndex == static_cast<int>(LEDEffectVelocity::DOUBLE_WAVE)) {  // double wave
    wave(length, true, color, 50, true);
    return rangeIndex;
  } else {
    return -1;
  }
}

void LightShowLED::startStandbyPalette(PaletteType paletteType) {
  _paletteType = paletteType;
  if (_paletteType == PaletteType::XMAS) {
    _paletteTheme = xmas_palettes;
    _paletteThemeLength = xmas_palette_count;
  }

  if (_paletteType == PaletteType::HALLOWEEN) {
    _paletteTheme = halloween_palettes;
    _paletteThemeLength = halloween_palette_count;
  }

  if (_paletteType == PaletteType::USA) {
    _paletteTheme = rwb_palettes;
    _paletteThemeLength = rwb_palette_count;
  }

  _standByPaletteOn = true;
  _standByPaletteIndex = 0;
  _currentPalette = _paletteTheme[_standByPaletteIndex];
}

void LightShowLED::stopStandByPalette() {
  _standByPaletteOn = false;
  off();
}

void LightShowLED::runStandBy() {
  if (_standByPaletteOn == true) {
    // send the 'leds' array out to the actual LED strip
    rainbow();
    FastLED.show();
    // insert a delay to keep the framerate modest
    FastLED.delay(1000 / 120);

    // do some periodic updates
    EVERY_N_MILLISECONDS(32) {
      _gHue = (_gHue + 1) % 256;
    }  // slowly cycle the "base color" through the rainbow
    // EVERY_N_SECONDS(30) {
    //   nextPattern();
    // }  // change patterns p
  }
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void LightShowLED::nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  _standByPaletteIndex = (_standByPaletteIndex + 1) % _paletteThemeLength;
  _currentPalette = _paletteTheme[_standByPaletteIndex];
}

void LightShowLED::rainbow() {
  fill_palette(_leds, _numLEDs, _gHue, 3, _currentPalette, 255, LINEARBLEND);
}
