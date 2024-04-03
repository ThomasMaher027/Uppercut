#ifndef SERIALCOM_H
#define SERIALCOM_H
#include <Arduino.h>

void recvWithStartEndMarkers();
void parseData(float* msg);
void showParsedData(float* msg);
bool getSerialMessage(float* msg); 

#endif SERIALCOM_H;
