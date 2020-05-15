#ifndef NOISE_H_
#define NOISE_H_

#include <FastLED.h>

const uint8_t kMatrixWidth = 21;
const uint8_t kMatrixHeight = 4;

#define MAX_DIMENSION 21
#define NUM_NOISE_LEDS (kMatrixWidth * kMatrixHeight)

// The leds
extern CRGB noiseLeds[NUM_NOISE_LEDS];

uint8_t XY(uint8_t x, uint8_t y);

void fillnoise8();
void setupNoise();
void noiseLoop();

#endif