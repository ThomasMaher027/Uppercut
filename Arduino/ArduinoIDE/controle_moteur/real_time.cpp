//#include <Arduino.h>
#include "real_time.h"
//#include <Dynamixel2Arduino.h>
using namespace ControlTableItem;

 
const int nb_motor = 4;
const uint8_t DXL_ID[nb_motor] = {1, 20, 3, 4}; //Motor ID
float min_pos_motor[nb_motor] = {115, 120, 180, 45};
float max_pos_motor[nb_motor] = {278, 258, 270, 235};
float home_pos[nb_motor] = {190.0, 258.0, 180.0, 235.0};
float max_vit = 0.8;
float lower_speed_limit = 0;


void changeAngle(float* target){
  target[0] = 360 - target[0];
  target[1] = 180 - target[1];
  target[2] = 180 - target[2];
  target[0] = map(target[0],270,360,min_pos_motor[0], 190);
  target[1] = map(target[1],0,90,(max_pos_motor[1]-90), max_pos_motor[1]);
  target[2] = map(target[2],0,180,min_pos_motor[2], max_pos_motor[2]);
}

void limitPosition(float *target){
  for(int i=0;i<(nb_motor-1);i++){
    if (target[i] < min_pos_motor[i]){
      target[i] = min_pos_motor[i];
    }
    if (target[i] > max_pos_motor[i]){
      target[i] = max_pos_motor[i];
    }
  }
    if (target[3] >= 1){
    target[3] = min_pos_motor[3];
  }
  else{
    target[3] = max_pos_motor[3];
  }
}

void setAngularSpeed(Dynamixel2Arduino dxl, float *target, int ind){
  float erreur_pos[(nb_motor-1)] = {0,0,0};
  float max_erreur = 0;
  for(int i=0;i<(nb_motor-1);i++){
    float present_pos = dxl.getPresentPosition(DXL_ID[i], UNIT_DEGREE);
    erreur_pos[i] = abs(target[i+ind] - present_pos);
    if (erreur_pos[i] > max_erreur) {
      max_erreur = erreur_pos[i];
    }
  }
  float gain = 0.9/max_erreur;
  for(int i=0;i<(nb_motor-1);i++){
    float speed_pct = erreur_pos[i]*gain;
    set_speed(dxl, DXL_ID[i], speed_pct);
  }
}


void setAngularPosition(Dynamixel2Arduino dxl, float *target, int ind){
  for (int i=0;i<nb_motor;i++){
    dxl.setGoalPosition(DXL_ID[i], target[i+ind], UNIT_DEGREE);
  }
}

/*
void setAngularPosition(Dynamixel2Arduino dxl, float *target, int ind){
  int motor=0;
  for(int i=ind;i<ind+nb_motor;i++){
    dxl.setGoalPosition(DXL_ID[motor], target[i+ind], UNIT_DEGREE);
    motor++;
  }
}*/

void set_speed(Dynamixel2Arduino dxl, uint8_t id, float speedPct) {
  if (speedPct > max_vit){
    speedPct = max_vit;
  }
  if (speedPct < lower_speed_limit){
    speedPct = lower_speed_limit;
  }
  double maxDynamixelSpeed = 1023*0.229; //RPM
  uint32_t newSpeedRpm = speedPct*maxDynamixelSpeed;
  uint32_t writeTimeout = 100; //ms
  dxl.writeControlTableItem(PROFILE_VELOCITY, id, newSpeedRpm, writeTimeout);
}

void homing(Dynamixel2Arduino dxl){
  setAngularPosition(dxl, home_pos, 0);
}
