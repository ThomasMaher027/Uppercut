#ifndef REAL_TIME_H
#define REAL_TIME_H
#include <Arduino.h>
#include <Dynamixel2Arduino.h>

void limitePosition(float *cibles);
// float calculVitesse(int moteur, float cibles);
void defVitesseAng(Dynamixel2Arduino dx1, float *cibles, int ind);
void defPosAng(Dynamixel2Arduino dx1, float *cibles, int ind);
void changeAngle(float* cibles);
void defVitesse(Dynamixel2Arduino dxl, uint8_t id, float pct_vitesse);
void homing(Dynamixel2Arduino dxl);

#endif REAL_TIME_H
