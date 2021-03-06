#include "api.h"
#include "led_control.h"
#include "common.h"
#include <FastLED.h>

WiFiServer server(80);
Application app;

/* ROUTER API Functions  */
void readPower(Request &req, Response &res) {
  res.print(isOff ? "off" : "on");
}

void readBrightness(Request &req, Response &res) {
  res.print(isBright ? "normal" : "low");
}

void updatePower(Request &req, Response &res) {
  String body = req.readString();
  Serial.println(body);
  if (body == "on") {
    turnOn();
    res.println("on");

  } else {
    turnOff();
    res.println("off");
  }
}

void updateHue(Request &req, Response &res) {
  String body = req.readString();
  uint8_t hue = body.toInt();
  //setRingColors(hue);
  //setMasterHue(hue);
  res.println("set");
}

void updateBrightness(Request &req, Response &res) {
  String body = req.readString();
  if(body == "low") {
    FastLED.setBrightness(30);
    isBright = false;
  } else {
    FastLED.setBrightness(255);
    isBright = true;
  }
  res.println(body);
}

void accessMiddleware(Request &req, Response &res) {
  res.set("Access-Control-Allow-Origin", "*");
}

void initApi() {
  app.use(&accessMiddleware);
  app.get("/power", &readPower);
  app.post("/power", &updatePower);
  app.post("/color/hue", &updateHue);
  app.get("/brightness", &readBrightness);
  app.post("/brightness", &updateBrightness);
  // app.route(staticFiles());
  server.begin();
}