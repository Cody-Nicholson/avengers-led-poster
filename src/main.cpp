#define FASTLED_ESP8266_NODEMCU_PIN_ORDER

#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <FastLED.h>
#include <creds.h>
#include <ota.h>

#include <string>

#include "api.h"
#include "comet.h"
#include "common.h"

ESP8266WiFiMulti wifiMulti;
WiFiServer server(80);
Application app;

boolean wasOff = false;
boolean wasOn = false;
bool isOff = false;

#define COMET_TR_PIN 1
#define COMET_TL_PIN 2
#define SHIELD_STAR_PIN 3
#define STAR_A_COMET_L_PIN 4

#define NUM_COMET_TR_LEDS 65
#define NUM_COMET_TR_A_LEDS 16
#define NUM_COMET_TR_B_LEDS 30
#define NUM_COMET_TR_C_LEDS 19

#define NUM_COMET_TL_THANOS_WAR_MACHINE_TOTAL 49
#define NUM_COMET_TL_A_LEDS 7
#define NUM_COMET_TL_B_LEDS 15
#define NUM_COMET_TL_C_LEDS 18
#define NUM_THANOS_LEDS 7
#define NUM_WAR_MACHINE_LEDS 2


#define NUM_SHIELD_STAR_B_TOTAL 18
#define NUM_SHIELD_LEDS 16
#define NUM_STAR_B_LEDS 2

#define NUM_STAR_A_COMET_L_TOTAL 46
#define NUM_STAR_A_LEDS 3
#define NUM_COMET_L_A_LEDS 16
#define NUM_COMET_L_B_LEDS 10
#define NUM_COMET_L_C_LEDS 17

uint8_t cometBrightness = 180;

/* Top Right Comets */
CRGB cometTrLeds[NUM_COMET_TR_LEDS];
CRGB *cometTrALeds = cometTrLeds;
CRGB *cometTrBLeds = &cometTrLeds[NUM_COMET_TR_A_LEDS];
CRGB *cometTrCLeds = &cometTrLeds[NUM_COMET_TR_A_LEDS + NUM_COMET_TR_B_LEDS];

/* Top Left Comets + Thanos + War Machine */
CRGB topLeftLeds[NUM_COMET_TL_THANOS_WAR_MACHINE_TOTAL];
CRGB *cometTlALeds = topLeftLeds;
CRGB *cometTlBLeds = &topLeftLeds[NUM_COMET_TL_A_LEDS];
CRGB *cometTlCLeds = &topLeftLeds[NUM_COMET_TL_A_LEDS + NUM_COMET_TL_B_LEDS];
CRGB *thanosLeds = &topLeftLeds[NUM_COMET_TL_A_LEDS + NUM_COMET_TL_B_LEDS + NUM_COMET_TL_C_LEDS];
CRGB *warMachineLeds = &topLeftLeds[NUM_COMET_TL_A_LEDS + NUM_COMET_TL_B_LEDS + NUM_COMET_TL_C_LEDS + NUM_THANOS_LEDS];

/* Shield Line + Star bottom */
CRGB shieldStarBLeds[NUM_SHIELD_STAR_B_TOTAL];
CRGB *starBLeds = &shieldStarBLeds[NUM_SHIELD_LEDS];

CRGB starACometLeftLeds[NUM_STAR_A_COMET_L_TOTAL];
CRGB *starALeds = &starACometLeftLeds[0];
CRGB *cometLeftALeds = &starACometLeftLeds[NUM_STAR_A_LEDS];
CRGB *cometLeftBLeds = &starACometLeftLeds[NUM_STAR_A_LEDS + NUM_COMET_L_A_LEDS];
CRGB *cometLeftCLeds = &starACometLeftLeds[NUM_STAR_A_LEDS + NUM_COMET_L_A_LEDS + NUM_COMET_L_B_LEDS];


uint8_t hueValue = 130;

void fill_solid(struct CRGB *targetArray, int startFill, int numToFill,
                const struct CHSV &hsvColor) {
  for (int i = startFill; i < startFill + numToFill; i++) {
    targetArray[i] = hsvColor;
  }
}

void initWifi() {
  wifiMulti.addAP(ROUTER_SSID, ROUTER_PASS);

  Serial.println("\nConnecting ...");
  while (wifiMulti.run() != WL_CONNECTED) {  // Wait for the Wi-Fi to connect
    delay(250);
    Serial.print('.');
  }

  Serial.print("\nConnected to ");
  Serial.println(WiFi.SSID());  // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(
      WiFi.localIP());  // Send the IP address of the ESP8266 to the computer
}

void initApi() {
  app.use(&accessMiddleware);
  app.get("/power", &readPower);
  app.post("/power", &updatePower);
  app.post("/color/hue", &updateHue);
  app.post("/brightness", &updateBrightness);
  // app.route(staticFiles());
  server.begin();
}

void fadeComet(CRGB *cometLeds, uint8_t numLeds, uint8_t fadeVal) {
  for (int i = 0; i < numLeds; i++) {
    cometLeds[i].fadeToBlackBy(fadeVal);
  }
}

void setLed(CRGB *cometLeds, uint8_t position, uint8_t numLeds, CHSV color) {
  cometLeds[numLeds - position - 1] = color;
}

void cometAnim(Comet &comet) {
  comet.setLed(CHSV(hueValue, 255, cometBrightness));
  /* Set Trail Color */
  uint8_t brightness = random(40, 80);
  comet.setLed(comet.position - 1, CHSV(hueValue + 40, 255, brightness));
  comet.fadeAll(8);
  comet.position++;

  if (comet.position >= comet.numLeds + 20) {
    comet.position = 0;
  }
}

// void cometAnim(CRGB *cometLeds, uint8_t numLeds) {
//   static uint8_t position = 0;
//   uint8_t prevPosition;

//   EVERY_N_MILLISECONDS_I(thisTimer, 60) {
//     if (position < numLeds) {
//       // cometLeds[position] = CHSV(hueValue, 255, 180);
//       setLed(cometLeds, position, numLeds, CHSV(hueValue, 255, 180));
//     }
//     prevPosition = position - 1;
//     /* Set Trail Color */
//     if (prevPosition >= 0 && prevPosition < numLeds) {
//       uint8_t brightness = random(40, 80);
//       // cometLeds[prevPosition] = CHSV(hueValue + 40, 255, brightness);
//       setLed(cometLeds, prevPosition, numLeds,
//              CHSV(hueValue + 40, 255, brightness));
//     }

//     fadeComet(cometLeds, numLeds, 8);
//     position++;
//     if (position >= numLeds + 20) {
//       position = 0;
//     }
//   }
// }

void setup() {
  Serial.begin(115200);
  delay(10);
  initWifi();
  initOTA();
  initApi();

  FastLED.setMaxPowerInVoltsAndMilliamps(5, 900);
  FastLED.addLeds<NEOPIXEL, COMET_TR_PIN>(cometTrLeds, NUM_COMET_TR_LEDS);
  FastLED.addLeds<NEOPIXEL, COMET_TL_PIN>(topLeftLeds, NUM_COMET_TL_THANOS_WAR_MACHINE_TOTAL);
  FastLED.addLeds<NEOPIXEL, SHIELD_STAR_PIN>(shieldStarBLeds, NUM_SHIELD_STAR_B_TOTAL);
  FastLED.addLeds<NEOPIXEL, STAR_A_COMET_L_PIN>(starACometLeftLeds, NUM_STAR_A_COMET_L_TOTAL);
}

void animThanos() {
  fill_solid(thanosLeds, 0, NUM_THANOS_LEDS, CHSV(HUE_PURPLE, 255, 90));
}

void breathLoop() {
  float breath = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 108.0;
  breath = map(breath, 0, 255, 1, 255);
  warMachineLeds[0] = CHSV(HUE_RED, 255, breath);
  warMachineLeds[1] = CHSV(HUE_RED, 255, breath);
}

/* BREATHE TEST 2 */
static float pulseSpeed = 1.4;  // Larger value gives faster pulse.

uint8_t hueA = 66;  // Start hue at valueMin.
uint8_t satA = 200;  // Start saturation at valueMin.
float valueMin = 120.0;  // Pulse minimum value (Should be less then valueMax).

uint8_t hueB = 83;  // End hue at valueMax.
uint8_t satB = 255;  // End saturation at valueMax.
float valueMax = 200.0;  // Pulse maximum value (Should be larger then valueMin).

uint8_t hue = hueA;  // Do Not Edit
uint8_t sat = satA;  // Do Not Edit
float val = valueMin;  // Do Not Edit
uint8_t hueDelta = hueA - hueB;  // Do Not Edit
static float delta = (valueMax - valueMin) / 2.35040238;  // Do Not Edit

void runBreathe() {
  /*  Formula (exp(sin(x)) - 1/e) * 255 / (e - 1/e)  */
  float dV = ((exp(sin(pulseSpeed * millis()/2000.0*PI)) -0.36787944) * delta);
  val = valueMin + dV;
  hue = map(val, valueMin, valueMax, hueA, hueB);  // Map hue based on current val
  sat = map(val, valueMin, valueMax, satA, satB);  // Map sat based on current val

  fill_solid(thanosLeds, 0, NUM_THANOS_LEDS, CHSV(hue, sat, val));
}

/*  BREATH END   */


/* Gloabal Modifiers  */

Comet cometTrA(cometTrALeds, NUM_COMET_TR_A_LEDS, true);
Comet cometTrB(cometTrBLeds, NUM_COMET_TR_B_LEDS, false);
Comet cometTrC(cometTrCLeds, NUM_COMET_TR_C_LEDS, true);

Comet cometTlA(cometTlALeds, NUM_COMET_TL_A_LEDS, true);
Comet cometTlB(cometTlBLeds, NUM_COMET_TL_B_LEDS, false);
Comet cometTlC(cometTlCLeds, NUM_COMET_TL_C_LEDS, true);

Comet cometLeftA(cometLeftALeds, NUM_COMET_L_A_LEDS, true);
Comet cometLeftB(cometLeftBLeds, NUM_COMET_L_B_LEDS, false);
Comet cometLeftC(cometLeftCLeds, NUM_COMET_L_C_LEDS, false);

void loop() {
  // cometAnim(cometALeds, NUM_COMET_TR_LEDS);
  //fill_solid(cometTrLeds, 0, NUM_COMET_TR_LEDS, CHSV(20,20,20));
  //fill_solid(shieldStarBLeds, 0, NUM_SHIELD_STAR_B_TOTAL, CHSV(20, 255, 30));
  //fill_solid(starACometLeftLeds, 0, NUM_STAR_A_COMET_L_TOTAL, CHSV(70,255,20));

  // EVERY_N_MILLISECONDS_I(t1, 60) { cometAnim(cometTrA); };
  // EVERY_N_MILLISECONDS_I(t2, 60) { cometAnim(cometTrB); };
  // EVERY_N_MILLISECONDS_I(t3, 60) { cometAnim(cometTrC); };

  // EVERY_N_MILLISECONDS_I(t4, 60) { cometAnim(cometTlA); };
  // EVERY_N_MILLISECONDS_I(t5, 60) { cometAnim(cometTlB); };
  // EVERY_N_MILLISECONDS_I(t6, 60) { cometAnim(cometTlC); };

  // EVERY_N_MILLISECONDS_I(t7, 60) { cometAnim(cometLeftA); };
  // EVERY_N_MILLISECONDS_I(t8, 60) { cometAnim(cometLeftB); };
  // EVERY_N_MILLISECONDS_I(t9, 60) { cometAnim(cometLeftC); };

  // cometAnim(cometTrB);
  //animThanos();
  //breathLoop();

  EVERY_N_MILLISECONDS_I(t1, 20) { 
    runBreathe();
  };

  FastLED.show();
  WiFiClient client = server.available();
  if (client.connected()) {
    app.process(&client);
  }
  ArduinoOTA.handle();

  // if (!isOff) {

  // }
}