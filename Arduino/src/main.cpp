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
void limitPosition(int id, float target);

const int nb_motor = 1;
const int nb_movement = 5;
const uint8_t DXL_ID[nb_motor] = {20}; //Motor ID

struct structStance{ 
  int stance_id;
  int nb_sub_movement;
  float target[16]; // motor position (in degree)
};

struct structStance stancePosition[nb_movement] = {
  // Home
  {0, 1 ,{0.0, 0.0, 0.0, 0.0, 
          0.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 0.0, 0.0, 
          0.0, 0.0, 0.0, 0.0}},

  // Curl
  {1, 1, {90.0, 90.0, 90.0, 90.0, 
          0.0, 0.0, 90.0, 0.0,
          0.0, 0.0, 90.0, 0.0,
          0.0, 0.0, 90.0, 0.0}},

  // Jab
  {2, 1, {10.0, 50.0, 90.0, 10.0, 
          50.0, 170.0, 50.0, 50.0, 
          50.0, 30.0, 75.0, 25.0,
          90.0, 75.0, 25.0, 25.0}},

  // Question
  {3, 1, {50.0, 170.0, 10.0, 10.0, 
          50.0, 50.0, 50.0, 50.0, 
          50.0, 30.0, 75.0, 25.0,
          90.0, 75.0, 25.0, 25.0}},

  // Corde
  {4, 1, {90.0, 0.0, 90.0, 90.0, 
          90.0, 90.0, 0.0, 0.0, 
          90.0, 90.0, 90.0, 0.0,
          90.0, 90.0, 90.0, 90.0}}
};

struct structStance stanceSpeed[nb_movement] = {
  // Home
  {0, 1 ,{0.3, 0.3, 0.3, 0.3, 
          0.3, 0.3, 0.3, 0.3, 
          0.3, 0.3, 0.3, 0.3, 
          0.3, 0.3, 0.3, 0.3}},
  
  // Curl
  {1, 1, {0.2, 0.3, 0.5, 0.3,
          0.3, 0.3, 0.3, 0.3,
          0.3, 0.3, 0.3, 0.3, 
          0.3, 0.3, 0.3, 0.3}},

  // Jab
  {2, 1, {0.1, 0.2, 0.3, 0.4, 
          0.5, 0.4, 0.3, 0.2, 
          0.3, 0.3, 0.3, 0.3, 
          0.3, 0.3, 0.3, 0.3}},

  // Question
  {3, 1, {0.1, 0.2, 0.3, 0.4, 
          0.5, 0.4, 0.3, 0.2, 
          0.3, 0.3, 0.3, 0.3, 
          0.3, 0.3, 0.3, 0.3}},
              
  // Corde        
  {4, 1, {0.1, 0.2, 0.3, 0.4, 
          0.5, 0.4, 0.3, 0.2, 
          0.3, 0.3, 0.3, 0.3, 
          0.3, 0.3, 0.3, 0.3}}
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
  homing();
  
}



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
        setGoalSpeed(stanceSpeed[chosen_command].target,i);
        setGoal_Position(stancePosition[chosen_command].target,i);
        check_pos_achieved(stancePosition[chosen_command].target,i);
      }
      homing();
    }
}


void setGoal_Position(float *target, int pos){
  int motor=0;
  for(int i=pos;i<pos+nb_motor;i++){
    dxl.setGoalPosition(DXL_ID[motor], target[i], UNIT_DEGREE);
    motor++;
  }
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
  setGoalSpeed(stanceSpeed[0].target,0);
  setGoal_Position(stancePosition[0].target,0);
  check_pos_achieved(stancePosition[0].target, 0);
}

void set_speed(uint8_t id, float speedPct) {
  if (speedPct > 1){
    speedPct = 1;
  }
  if (speedPct < 0){
    speedPct = 0;
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

void limitPosition(int id, float target){

}


int choiceMovement(int command){
  for (int i=0;i<nb_movement;i++){
    if (command == stancePosition[i].stance_id){
      return i;
    } 
  }
  return -1;
}