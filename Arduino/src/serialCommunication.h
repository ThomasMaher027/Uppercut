#ifndef SERIALCOM_H
#define SERIALCOM_H
#include <Arduino.h>

void recvWithStartEndMarkers();
void parseData();
void showParsedData();
float* getSerialMessage(); 

#endif SERIALCOM_H