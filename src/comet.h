#include <FastLED.h>

class Comet {
 public:
  Comet(CRGB *cometLeds, uint8_t num) {
    leds = cometLeds;
    position = 0;
    numLeds = num;
    reversed = false;
  }

  Comet(CRGB *cometLeds, uint8_t num, bool rev) {
    leds = cometLeds;
    numLeds = num;
    reversed = rev;
    position = 0;
  }

  void setNext(CHSV color) {}

  void setLed(uint8_t pos, CHSV color) {
    if (pos >= 0 && pos < numLeds) {
      if (reversed) {
        leds[numLeds - 1 - pos] = color;
      } else {
        leds[pos] = color;
      }
    }
  }

  void setLed(CHSV color) { setLed(position, color); }

  void fadeAll(uint8_t fadeVal) {
    for (int i = 0; i < numLeds; i++) {
      leds[i].fadeToBlackBy(fadeVal);
    }
  }

  uint8_t numLeds;
  uint8_t position;

 private:
  CRGB *leds;
  bool reversed;
};