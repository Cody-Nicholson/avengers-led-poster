// #include "noise.h"


// // The 32bit version of our coordinates
// static uint16_t x;
// static uint16_t y;
// static uint16_t z;

// CRGB noiseLeds[NUM_NOISE_LEDS];

// // We're using the x/y dimensions to map to the x/y pixels on the matrix.  We'll
// // use the z-axis for "time".  speed determines how fast time moves forward. Try
// // 1 for a very slow moving effect, or 60 for something that ends up looking
// // like water. uint16_t speed = 1; // almost looks like a painting, moves very
// // slowly
// uint16_t noiseSpeed = 10;  // a nice starting speed, mixes well with a scale of 100
// // uint16_t speed = 33;
// // uint16_t speed = 100; // wicked fast!

// // Scale determines how far apart the pixels in our noise matrix are.  Try
// // changing these values around to see how it affects the motion of the display.
// // The higher the value of scale, the more "zoomed out" the noise iwll be.  A
// // value of 1 will be so zoomed in, you'll mostly see solid colors.

// // uint16_t scale = 1; // mostly just solid colors
// // uint16_t scale = 4011; // very zoomed out and shimmery
// uint16_t noiseScale = 2000;

// // This is the array that we keep our computed noise values in
// uint8_t noiseList[21][21];

// void fillnoise8() {
//   for (int i = 0; i < MAX_DIMENSION; i++) {
//     int ioffset = noiseScale * i;
//     for (int j = 0; j < MAX_DIMENSION; j++) {
//       int joffset = noiseScale * j;
//       noiseList[i][j] = inoise8(x + ioffset, y + joffset, z);
//     }
//   }
//   z += noiseSpeed;
// }

// uint8_t XY(uint8_t x, uint8_t y) {
//   uint8_t i;

//   if (y & 0x01) {
//     // Odd rows run backwards
//     uint8_t reverseX = (kMatrixWidth - 1) - x;
//     i = (y * kMatrixWidth) + reverseX;
//   } else {
//     // Even rows run forwards
//     i = (y * kMatrixWidth) + x;
//   }

//   return i;
// }

// void setupNoise() {
//   x = random16();
//   y = random16();
//   z = random16();
// }

// void noiseLoop() {
//   static uint8_t ihue = 0;
//   fillnoise8();
//   for (int i = 0; i < kMatrixWidth; i++) {
//     for (int j = 0; j < kMatrixHeight; j++) {
//       // We use the value at the (i,j) coordinate in the noise
//       // array for our brightness, and the flipped value from (j,i)
//       // for our pixel's hue.
//       noiseLeds[XY(i, j)] = CHSV(noiseList[j][i], 255, noiseList[i][j]);
//       // You can also explore other ways to constrain the hue used, like below
//       //noiseLeds[XY(i,j)] = CHSV(ihue + (noiseList[j][i]>>2),255,noiseList[i][j]);
//     }
//   }
//   ihue += 1;
// }

