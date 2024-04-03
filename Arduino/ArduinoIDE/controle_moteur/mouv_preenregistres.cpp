#include "mouv_preenregistres.h"
using namespace ControlTableItem;

const int nb_motor = 4;
const uint8_t DXL_ID[nb_motor] = {1, 20, 3, 4}; //Motor ID


const int nb_movement = 3;
int nb_target = 12;
struct structStance{ 
  int stance_id;
  int nb_sub_movement;
  float target[12]; // motor position (in degree)
};

struct structStance stancePosition[nb_movement] = {
  // Home
  
  {0, 1 ,{190.0, 258.0, 70.0,
          200.0, 140.0, 150.0,
          200.0, 140.0, 150.0,
          200.0, 140.0, 150.0}},

  // Curl
  {1, 1, {110.0, 140.0, 325.0, 
          150.0, 140.0, 280.0, 
          150.0, 140.0, 280.0, 
          150.0, 140.0, 280.0}},

  // Jab
  {2, 3, {196.0, 50.0, 305.0, 
          110.4, 50.0, 178.5, 
          196.0, 50.0, 305.0,
          196.0, 50.0, 305.0}},
};

struct structStance stanceSpeed[nb_movement] = {
  // Home
  {0, 1 ,{0.3, 0.3, 0.3,
          0.3, 0.3, 0.3,
          0.3, 0.3, 0.3, 
          0.3, 0.3, 0.3}},
  
  // Curl
  {1, 1, {0.2, 0.3, 0.2,
          0.3, 0.3, 0.3,
          0.3, 0.3, 0.3, 
          0.3, 0.3, 0.3}},

  // Jab
  {2, 3, {0.35, 0.35, 0.6, 
          0.35, 0.35, 0.6,
          0.35, 0.35, 0.6,
          0.35, 0.35, 0.6,}},
};

void check_pos_achieved(Dynamixel2Arduino dxl, float *target, int j){
  bool ready = false;
  float interval = 2.0;
  while (ready == false){
    int total = 0;
    int target_index = j;
    for (int i=0;i<nb_motor;i++){
      float pos_motor = dxl.getPresentPosition(DXL_ID[i], UNIT_DEGREE);
      if (abs(target[target_index]-pos_motor)<interval){
        total += 1;
      }
      target_index += 1;
    }
    if (total == nb_motor){
      ready = true;
    }
    else if (total > nb_motor){
      Serial.println("Le check pour savoir si les moteurs sont arrives a destination n'est pas bien fait");
    }
  }
}

void envoiCommande(Dynamixel2Arduino dxl, int chosen_command){
  if (chosen_command < nb_movement){
      for (int i=0; i<stancePosition[chosen_command].nb_sub_movement*nb_motor; i+=nb_motor){
          setAngularSpeed(dxl, stancePosition[chosen_command].target, i);
          setAngularPosition(dxl, stancePosition[chosen_command].target, i);
          check_pos_achieved(dxl, stancePosition[chosen_command].target, i);
      homing(dxl);
    }
  }
}


/*
void setGoal_Position(Dynamixel2Arduino dxl, float *target, int pos){
  int motor=0;
  for(int i=pos;i<pos+nb_motor;i++){
    float speed_pct = getSpeed(dxl, motor, target[i]);
    set_speed(dxl, DXL_ID[motor], speed_pct);
    dxl.setGoalPosition(DXL_ID[motor], target[i], UNIT_DEGREE);
    motor++;
  }
}*/
/*
float getSpeed(Dynamixel2Arduino dxl, int motor, float target){
  // Égal à 1/220. Donc, pour une erreur d'angle de 220 deg, la vitesse est 100% (les moteurs ne tourneront pas plus de 270 deg) 
  float p_gain = 0.004545455;
  float error = abs(dxl.getPresentPosition(DXL_ID[motor], UNIT_DEGREE) - target);
  float out_speed = error*p_gain;
  return out_speed; 
}*/
/*
void setGoalSpeed(Dynamixel2Arduino dxl, float *target, int pos){
  int motor=0;
  for(int i=pos;i<pos+nb_motor;i++){
    set_speed(dxl, DXL_ID[motor], target[i]);
    motor++;
  }
}*/



/*
void printPosition(Dynamixel2Arduino dxl){
  for(int i=0;i<nb_motor;i++){
    float present_position = dxl.getPresentPosition(DXL_ID[i], UNIT_DEGREE);
    DEBUG_SERIAL.print("Present_Position ");
    DEBUG_SERIAL.print(i);
    DEBUG_SERIAL.print(" (degree) : ");
    DEBUG_SERIAL.println(present_position);
  }
}
*/
/*
int choiceMovement(int command){
  for (int i=0;i<nb_movement;i++){
    if (command == stancePosition[i].stance_id){
      return i;
    } 
  }
  return -1;
}*/
