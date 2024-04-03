#ifndef MOUV_PREENREGISTRES_H
#define MOUV_PREENREGISTRES_H
#include <Arduino.h>
#include <Dynamixel2Arduino.h>
#include "real_time.h"


void check_pos_achieved(Dynamixel2Arduino dxl, float *target, int i);
void envoiCommande(Dynamixel2Arduino dxl, int chosen_command);
/*
void setGoal_Position(Dynamixel2Arduino dxl, float *target, int pos);
void setGoalSpeed(Dynamixel2Arduino dxl, float *target, int pos);
float getSpeed(Dynamixel2Arduino dxl, int motor, float target);*/




#endif MOUV_PREENREGISTRES_H
