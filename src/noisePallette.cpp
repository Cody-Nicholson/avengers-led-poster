#include "noisePallette.h"

// The 32bit version of our coordinates
static uint16_t x;
static uint16_t y;
static uint16_t z;

#define OUT_OF_RANGE_NOISE 255

CRGB noisePaletteLeds[NUM_NOISE_LEDS];

uint16_t speed = 20;
uint16_t scale = 30;

// This is the array that we keep our computed noise values in
uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];

CRGBPalette16 currentPalette(PartyColors_p);
uint8_t colorLoop = 1;

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void fillnoise8() {
  // If we're runing at a low "speed", some 8-bit artifacts become visible
  // from frame-to-frame.  In order to reduce this, we can do some fast
  // data-smoothing. The amount of data smoothing we're doing depends on
  // "speed".
  uint8_t dataSmoothing = 0;
  if (speed < 50) {
    dataSmoothing = 200 - (speed * 4);
  }

  for (int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for (int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;

      uint8_t data = inoise8(x + ioffset, y + joffset, z);

      // The range of the inoise8 function is roughly 16-238.
      // These two operations expand those values out to roughly 0..255
      // You can comment them out if you want the raw noise data.
      data = qsub8(data, 16);
      data = qadd8(data, scale8(data, 39));

      if (dataSmoothing) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata =
            scale8(olddata, dataSmoothing) + scale8(data, 256 - dataSmoothing);
        data = newdata;
      }

      noise[i][j] = data;
    }
  }

  z += speed;

  // apply slow drift to X and Y, just for visual variation.
  x += speed / 8;
  y -= speed / 16;
}

void mapNoiseToLEDsUsingPalette() {
  static uint8_t ihue = 0;
  uint8_t ledIndex;

  for (int i = 0; i < kMatrixWidth; i++) {
    for (int j = 0; j < kMatrixHeight; j++) {
      // We use the value at the (i,j) coordinate in the noise
      // array for our brightness, and the flipped value from (j,i)
      // for our pixel's index into the color palette.

      uint8_t index = noise[j][i];
      uint8_t bri = noise[i][j];

      // if this palette is a 'loop', add a slowly-changing base value
      if (colorLoop) {
        index += ihue;
      }

      // brighten up, as the color palette itself often contains the
      // light/dark dynamic range desired
      if (bri > 127) {
        bri = 200;
      } else {
        bri = dim8_raw(bri * 2);
      }

      CRGB color = ColorFromPalette(currentPalette, index, bri);
      ledIndex = XY(i, j);
      if (ledIndex != OUT_OF_RANGE_NOISE && ledIndex >= 0 && ledIndex < NUM_NOISE_LEDS) {
        noisePaletteLeds[ledIndex] = color;
      }
    }
  }

  ihue += 1;
}

void setupNoise() {
  x = random16();
  y = random16();
  z = random16();
}

void noisePaletteloop() {
  // Periodically choose a new palette, speed, and scale
  ChangePaletteAndSettingsPeriodically();

  // generate noise data
  fillnoise8();

  // convert the noise data to colors in the LED array
  // using the current palette
  mapNoiseToLEDsUsingPalette();
}

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p,
// RainbowStripeColors_p, OceanColors_p, CloudColors_p, LavaColors_p,
// ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can
// write code that creates color palettes on the fly.

// 1 = 5 sec per palette
// 2 = 10 sec per palette
// etc
#define HOLD_PALETTES_X_TIMES_AS_LONG 8

void ChangePaletteAndSettingsPeriodically() {
  uint8_t secondHand = ((millis() / 1000) / HOLD_PALETTES_X_TIMES_AS_LONG) % 60;
  static uint8_t lastSecond = 99;

  if (lastSecond != secondHand) {
    lastSecond = secondHand;
    // if (secondHand == 0) {
    //   currentPalette = HeatColors_p;
    //   speed = 20;
    //   scale = 20;
    //   colorLoop = 0;
    // }
    if (secondHand == 0) {
      currentPalette = LavaColors_p;
      speed = 8;
      scale = 50;  // org 30
      colorLoop = 0;
    }
    // if (secondHand == 10) {
    //   SetupBlackAndWhiteStripedPalette();
    //   speed = 20;
    //   scale = 30;
    //   colorLoop = 1;
    // }
    // if (secondHand == 15) {
    //   currentPalette = OceanColors_p;
    //   speed = 20;
    //   scale = 20;
    //   colorLoop = 0;
    // }
    if (secondHand == 30) {  // Blue BG
      currentPalette = CloudColors_p;
      speed = 4;
      scale = 30;
      colorLoop = 0;
    }
    // if (secondHand == 25) {  // org laval
    //   currentPalette = LavaColors_p;
    //   speed = 8;
    //   scale = 50;
    //   colorLoop = 0;
    // }
    // if (secondHand == 30) {
    //   currentPalette = OceanColors_p;
    //   speed = 20;
    //   scale = 90;
    //   colorLoop = 0;
    // }
    // if (secondHand == 35) {
    //   currentPalette = PartyColors_p;
    //   speed = 20;
    //   scale = 30;
    //   colorLoop = 1;
    // }
  }
}

// This function generates a random palette that's a gradient
// between four different colors.  The first is a dim hue, the second is
// a bright hue, the third is a bright pastel, and the last is
// another bright hue.  This gives some visual bright/dark variation
// which is more interesting than just a gradient of different hues.
void SetupRandomPalette() {
  currentPalette =
      CRGBPalette16(CHSV(random8(), 255, 32), CHSV(random8(), 255, 255),
                    CHSV(random8(), 128, 255), CHSV(random8(), 255, 255));
}

/* XY Coords to Led matrix index  */
uint8_t XY(uint8_t x, uint8_t y) {
  uint8_t i;

  if (y & 0x01) {
    // Odd rows run backwards
    uint8_t reverseX = (kMatrixWidth - 1) - x;
    i = (y * kMatrixWidth) + reverseX;
  } else {
    // Even rows run forwards
    i = (y * kMatrixWidth) + x;
  }

  /* Special Logic for led strip having a longer 4th row */
  if (x >= 21 && y <= 2) {
    return OUT_OF_RANGE_NOISE;
  }

  if (y == 1 || y == 2) {
    return i - 4;
  }

  if (y >= 3) {
    return i - 6;
  }

  return i;
}