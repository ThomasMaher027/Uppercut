/*#include <Arduino.h>
#include "real_time.h"
#include <Dynamixel2Arduino.h>
const int nb_motor = 3;
const float min_pos_motor[nb_motor] = {115, 120, 70};
const float max_pos_motor[nb_motor] = {278, 258, 232};
const uint8_t DXL_ID[nb_motor] = {1, 20, 3}; //Motor ID


void changeAngle(float* target){
  target[0] = 360 - target[0];
  target[1] = 180 - target[1];
  target[2] = 90 - target[2];
  target[0] = map(target[0],270,360,min_pos_motor[0], 190);
  target[1] = map(target[1],0,90,(max_pos_motor[1]-90), max_pos_motor[1]);
  target[2] = map(target[2],0,180,min_pos_motor[2], max_pos_motor[2])*2;
}

void limitPosition(float *target_angle){
  for(int i=0;i<nb_motor;i++){
    if (target_angle[i] < min_pos_motor[i]){
      target_angle[i] = min_pos_motor[i];
    }
    if (target_angle[i] > max_pos_motor[i]){
      target_angle[i] = max_pos_motor[i];
    }
  }
}

void setAngularSpeed(Dynamixel2Arduino dx1, float *target){
  float erreur_pos[nb_motor] = {0,0,0};
  float max_erreur = 0;
  for(int i=0;i<nb_motor;i++){
    float present_pos = dxl.getPresentPosition(DXL_ID[i], UNIT_DEGREE);
    erreur_pos[i] = abs(target[i] - present_pos);
    if (erreur_pos[i] > max_erreur) {
      max_erreur = erreur_pos[i];
    }
  }
  float gain = 0.9/max_erreur;
  for(int i=0;i<nb_motor;i++){
    float speed_pct = erreur_pos[i]*gain;
    set_speed(DXL_ID[i], speed_pct);
  }
}

void setAngularPosition(Dynamixel2Arduino dx1, float *target){
  for (int i=0;i<nb_motor;i++){
    dxl.setGoalPosition(DXL_ID[i], target[i], UNIT_DEGREE);
  }
}*/
