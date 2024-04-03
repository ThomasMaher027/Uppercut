#ifndef REAL_TIME_H
#define REAL_TIME_H
#include <Arduino.h>
#include <Dynamixel2Arduino.h>

void limitPosition(float *target_angle);
float getSpeed(int motor, float target);
void setAngularSpeed(Dynamixel2Arduino dx1, float *target);
void setAngularPosition(Dynamixel2Arduino dx1, float *target);
void changeAngle(float* target);

#endif REAL_TIME_H
