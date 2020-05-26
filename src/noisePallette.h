#ifndef NOISE_PALETTE_H_
#define NOISE_PALETTE_H_

#include <FastLED.h>

const uint8_t kMatrixWidth = 23;
const uint8_t kMatrixHeight = 4;

#define MAX_DIMENSION 23
#define MISSING_FILL 6
// #define NUM_NOISE_LEDS (kMatrixWidth * kMatrixHeight)
#define NUM_NOISE_LEDS (kMatrixWidth * kMatrixHeight) - MISSING_FILL + 1

uint8_t XY(uint8_t x, uint8_t y);

extern CRGB noisePaletteLeds[NUM_NOISE_LEDS];

void noisePaletteloop();
void setupNoise();
void mapNoiseToLEDsUsingPalette();
void ChangePaletteAndSettingsPeriodically();
void SetupRandomPalette();
void SetupBlackAndWhiteStripedPalette();
void SetupPurpleAndGreenPalette();


#endif