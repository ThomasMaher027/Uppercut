#ifndef REAL_TIME_H
#define REAL_TIME_H
#include <Arduino.h>
#include <Dynamixel2Arduino.h>

 

void limitPosition(float *target);
float getSpeed(int motor, float target);
void setAngularSpeed(Dynamixel2Arduino dx1, float *target, int ind);
void setAngularPosition(Dynamixel2Arduino dx1, float *target, int ind);
void changeAngle(float* target);
void set_speed(Dynamixel2Arduino dxl, uint8_t id, float speedPct);
void homing(Dynamixel2Arduino dxl);

//void set_speed(uint8_t id, float speedPct);
/*
void homing();
//void printPosition();
//int choiceMovement(int command);
void check_pos_achieved(Dynamixel2Arduino dxl, float *target, int i);
void setGoal_Position(Dynamixel2Arduino dxl, float *target, int pos);
void setGoalSpeed(Dynamixel2Arduino dxl, float *target, int pos);
float getSpeed(Dynamixel2Arduino dxl, int motor, float target);
*/
#endif REAL_TIME_H
