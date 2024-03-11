/*******************************************************************************
* Copyright 2016 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/


#include <Arduino.h>
#include <Dynamixel2Arduino.h>
#include "serialCommunication.h"

//OpenRB does not require the DIR control pin.
#define DXL_SERIAL Serial1
#define DEBUG_SERIAL Serial
const int DXL_DIR_PIN = -1;

const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

//This namespace is required to use Control table item names
using namespace ControlTableItem;

// put function declarations here:
void set_speed(uint8_t id, float speedPct);
void homing();
void printPosition();
int choiceMovement(int command);
void check_pos_achieved(float *target, int i);
void setGoal_Position(float *target, int pos);
void setGoalSpeed(float *target, int pos);
void limitPosition(float *target_angle);
float getSpeed(int motor, float target);
void setAngularSpeed(float *target, float delta_time);
void setAngularPosition(float *target);

const int nb_motor = 3;
const int nb_movement = 4;
const uint8_t DXL_ID[nb_motor] = {1, 20, 3}; //Motor ID
float min_pos_motor[nb_motor] = {119, 92, 29};
float max_pos_motor[nb_motor] = {317, 234, 219};
float upper_speed_limit = 0.8;
float lower_speed_limit = 0.1;

int nb_target = 12;
struct structStance{ 
  int stance_id;
  int nb_sub_movement;
  float target[12]; // motor position (in degree)
};

struct structStance stancePosition[nb_movement] = {
  // Home
  {0, 1 ,{228.0, 92.0, 29.0,
          228.0, 92.0, 29.0, 
          228.0, 92.0, 29.0,
          228.0, 92.0, 29.0,}},

  // Curl
  {1, 1, {300.0, 92.0, 200.0, 
          300.0, 92.0, 200.0,
          300.0, 92.0, 200.0,
          300.0, 92.0, 200.0}},

  // Jab
  {2, 3, {228.0, 195.0, 219.0, 
          310.0, 195.0, 35.0, 
          228.0, 195.0, 219.0,
          228.0, 195.0, 219.0}},

  // Question
  {3, 1, {228.0, 92.0, 175.0,
          228.0, 92.0, 175.0, 
          228.0, 92.0, 175.0,
          228.0, 92.0, 175.0,}},
/*
  // Corde
  {4, 1, {90.0, 0.0, 90.0, 
          90.0, 90.0, 0.0, 
          90.0, 90.0, 90.0,
          90.0, 90.0, 90.0}}*/
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

  // Question
  {3, 1, {0.1, 0.2, 0.3, 
          0.2, 0.2, 0.3,
          0.3, 0.3, 0.3, 
          0.3, 0.3, 0.3}},
   /*           
  // Corde        
  {4, 1, {0.1, 0.2, 0.3,
          0.25, 0.25, 0.3, 
          0.3, 0.3, 0.3, 
          0.3, 0.3, 0.3}}*/
};


void setup() {
  // Use UART port of DYNAMIXEL Shield to debug.
  DEBUG_SERIAL.begin(115200);
  while(!DEBUG_SERIAL);

  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  
  // initialise each motor
  for (int i=0;i<nb_motor;i++){
    dxl.ping(DXL_ID[i]);
    dxl.torqueOff(DXL_ID[i]);
    dxl.setOperatingMode(DXL_ID[i], OP_POSITION);
    dxl.torqueOn(DXL_ID[i]);
    set_speed(DXL_ID[i],0.2);
  }
  //limitPosition();
  homing();
  
}


/*
// fonction loop() pour les mouvements préenregistés
void loop() {
  while (!Serial.available());
    int chosen_command = 0;
    chosen_command = Serial.readString().toInt();
    Serial.print(chosen_command);
    
    if (chosen_command <= 0){
      chosen_command = 0;
    }

    if (chosen_command < nb_movement){
      for (int i=0; i<stancePosition[chosen_command].nb_sub_movement*nb_motor; i+=nb_motor){
        //setGoalSpeed(stanceSpeed[chosen_command].target,i);
        setGoal_Position(stancePosition[chosen_command].target,i);
        check_pos_achieved(stancePosition[chosen_command].target,i);
      }
      homing();
    }
}*/

// fonction loop() pour le temps réel
void loop(){
  while (!Serial.available());
    // TODO : Lire et séparer les valeurs du port série
    // J'ai besoin des cibles d'angle pour les 4 moteurs et le temps entre du écriture du port série
    float target_angle[nb_motor] = {};
    float delta_time;
    limitPosition(target_angle);
    setAngularSpeed(target_angle, delta_time);
    setAngularPosition(target_angle);
}

void limitPosition(float *target_angle){
  for(int i=0;i<nb_motor;i++){
    if (target_angle[i] < min_pos_motor[i]){
      target_angle[i] = min_pos_motor[i]
    }
    if (target_angle[i] < min_pos_motor[i]){
      target_angle[i] = min_pos_motor[i]
    }
  }
}

void setAngularSpeed(float *target, float delta_time){
  float conv_rad_enc = 162.82; // (valeur d'encodeur)/rad
  for(int i=0;i<nb_motor;i++){
    float present_pos = dxl.getPresentPosition(DXL_ID[i], UNIT_DEGREE);
    float delta_pos = abs(target[i] - present_pos)*conv_rad_enc; // (valeur d'encodeur);
    if (delta_pos>1023){
      delta_pos = 1023;
    }
    float speed = delta_pos/delta_time; // dq/dt
    speed *= 0.229; // RPM/(valeur encodeur)
    dxl.writeControlTableItem(PROFILE_VELOCITY, i, speed, 100);
  }
}

void setAngularPosition(float *target){
  for (int i=0;i<nb_motor;i++){
    dxl.setGoalPosition(DXL_ID[i], target[i], UNIT_DEGREE);
  }
}

void setGoal_Position(float *target, int pos){
  int motor=0;
  for(int i=pos;i<pos+nb_motor;i++){
    float speed_pct = getSpeed(motor, target[i]);
    set_speed(DXL_ID[motor], speed_pct);
    dxl.setGoalPosition(DXL_ID[motor], target[i], UNIT_DEGREE);
    motor++;
  }
}

float getSpeed(int motor, float target){
  // Égal à 1/220. Donc, pour une erreur d'angle de 220 deg, la vitesse est 100% (les moteurs ne tourneront pas plus de 270 deg) 
  float p_gain = 0.004545455;
  float error = abs(dxl.getPresentPosition(DXL_ID[motor], UNIT_DEGREE) - target);
  float out_speed = error*p_gain;
  return out_speed; 
}

void setGoalSpeed(float *target, int pos){
  int motor=0;
  for(int i=pos;i<pos+nb_motor;i++){
    set_speed(DXL_ID[motor], target[i]);
    motor++;
  }
}

void check_pos_achieved(float *target, int j){
  bool ready = false;
  float interval = 5.0;
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

void homing(){
  //setGoalSpeed(stanceSpeed[0].target,0);
  setGoal_Position(stancePosition[0].target,0);
  check_pos_achieved(stancePosition[0].target, 0);
}

void set_speed(uint8_t id, float speedPct) {
  if (speedPct > upper_speed_limit){
    speedPct = upper_speed_limit;
  }
  if (speedPct < lower_speed_limit){
    speedPct = lower_speed_limit;
  }
  double maxDynamixelSpeed = 1023*0.229; //RPM
  uint32_t newSpeedRpm = speedPct*maxDynamixelSpeed;
  uint32_t writeTimeout = 100; //ms
  dxl.writeControlTableItem(PROFILE_VELOCITY, id, newSpeedRpm, writeTimeout);
}

void printPosition(){
  for(int i=0;i<nb_motor;i++){
    float present_position = dxl.getPresentPosition(DXL_ID[i], UNIT_DEGREE);
    DEBUG_SERIAL.print("Present_Position ");
    DEBUG_SERIAL.print(i);
    DEBUG_SERIAL.print(" (degree) : ");
    DEBUG_SERIAL.println(present_position);
  }
}


int choiceMovement(int command){
  for (int i=0;i<nb_movement;i++){
    if (command == stancePosition[i].stance_id){
      return i;
    } 
  }
  return -1;
}