/*#include "temps_reel.h"



temps_reel::temps_reel(int* id, float* min_pos, float* max_pos, float* home_pos){
  for(int ii=0;ii<nb_motor;ii++){
      DXL_ID[ii] = id[ii];
      min_pos_motor[ii] = min_pos[ii]; 
      max_pos_motor[nb_motor] = max_pos[ii];
  }
}



temps_reel::~temps_reel(){
}



void temps_reel::changeAngle(float* target){
  target[0] = 360 - target[0];
  target[1] = 180 - target[1];
  target[2] = 90 - target[2];
  target[0] = map(target[0],270,360,min_pos_motor[0], 190);
  target[1] = map(target[1],0,90,(max_pos_motor[1]-90), max_pos_motor[1]);
  target[2] = map(target[2],0,180,min_pos_motor[2], max_pos_motor[2])*2;
}



void temps_reel::limitPosition(float *target_angle){
  for(int i=0;i<nb_motor;i++){
    if (target_angle[i] < min_pos_motor[i]){
      target_angle[i] = min_pos_motor[i];
    }
    if (target_angle[i] > max_pos_motor[i]){
      target_angle[i] = max_pos_motor[i];
    }
  }
}



void temps_reel::setAngularSpeed(float *target){
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



void temps_reel::setAngularPosition(float *target){
  for (int i=0;i<nb_motor;i++){
    dxl.setGoalPosition(DXL_ID[i], target[i], UNIT_DEGREE);
  }
}



void temps_reel::set_speed(uint8_t id, float speedPct) {
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
}*/
