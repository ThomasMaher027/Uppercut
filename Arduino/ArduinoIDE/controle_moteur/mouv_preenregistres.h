#ifndef MOUV_PREENREGISTRES_H
#define MOUV_PREENREGISTRES_H
#include <Arduino.h>
#include <Dynamixel2Arduino.h>
#include "real_time.h"


void check_pos_achieved(Dynamixel2Arduino dxl, float *cibles, int i);
void envoiCommande(Dynamixel2Arduino dxl, int chosen_command);
/*
void setGoal_Position(Dynamixel2Arduino dxl, float *cibles, int pos);
void setGoalSpeed(Dynamixel2Arduino dxl, float *cibles, int pos);
float calculVitesse(Dynamixel2Arduino dxl, int moteur, float cibles);*/




#endif MOUV_PREENREGISTRES_H
