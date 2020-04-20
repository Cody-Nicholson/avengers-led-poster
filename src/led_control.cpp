#include "common.h"
#include "led_control.h"
#include <FastLED.h>

void turnOn() {
  isOff = false;
}

void turnOff() {
  isOff = true;
  FastLED.showColor(CRGB(0, 0, 0), 0);
  Serial.print("OFF");
}