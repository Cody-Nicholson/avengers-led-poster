#include "api.h"
#include "led_control.h"
#include "common.h"
#include <FastLED.h>


/* ROUTER API Functions  */
void readPower(Request &req, Response &res) {
  res.print(isOff ? "off" : "on");
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
  } else {
    FastLED.setBrightness(255);
  }
  res.println(body);
}

void accessMiddleware(Request &req, Response &res) {
  res.set("Access-Control-Allow-Origin", "*");
}