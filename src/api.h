#pragma once

#include "aWOT.h"

void readPower(Request &req, Response &res);
void readBrightness(Request &req, Response &res);
void updatePower(Request &req, Response &res);
void updateHue(Request &req, Response &res);
void updateBrightness(Request &req, Response &res);
void accessMiddleware(Request &req, Response &res);

void initApi();