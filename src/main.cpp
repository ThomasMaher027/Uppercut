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
const int nb_motor = 4;
const int nb_movement = 3;
const uint8_t DXL_ID[nb_motor] = {1, 3, 4, 20}; //Motor ID

const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

//This namespace is required to use Control table item names
using namespace ControlTableItem;

// put function declarations here:
void setSpeed(uint8_t id, float speedPct);
void homing();
void printPosition();
int choiceMovement(String command);
void check_pos_achieved(float *target);
void setGoal(float *pos_motor, int pos);

struct _stancePosition{ // motor position (in degree)
  char stance_name[20];
  int nb_mov;
  float pos_motor[16];
};

struct _stancePosition stancePosition[nb_movement] = {
  {"home", 1 ,{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
  {"curl", 1, {0.0, 0.0, 90.0, 0.0, 0.0, 0.0, 90.0, 0.0, 0.0, 0.0, 90.0, 0.0, 0.0, 0.0, 90.0, 0.0}},
  {"jab", 2, {10.0, 10.0, 10.0, 10.0, 50.0, 50.0, 50.0, 50.0, 50.0, 30.0, 75.0, 25.0, 90.0, 75.0, 25.0}}
};

struct movement {
  char pos_name[20];
  int nb_mov;
  float pos_motor[16];
};

//struct movement curl = {"curl", 1, {0.0, 0.0, 90.0, 0.0, 0.0, 0.0, 90.0, 0.0, 0.0, 0.0, 90.0, 0.0, 0.0, 0.0, 90.0, 0.0}};


void setup() {
  // Use UART port of DYNAMIXEL Shield to debug.
  DEBUG_SERIAL.begin(115200);
  while(!DEBUG_SERIAL);

  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  

  for (int i=0;i<nb_motor;i++){
    dxl.ping(DXL_ID[i]);
    dxl.torqueOff(DXL_ID[i]);
    dxl.setOperatingMode(DXL_ID[i], OP_POSITION);
    dxl.torqueOn(DXL_ID[i]);
    setSpeed(DXL_ID[i],0.2);
  }
  homing();
}



void loop() {
  String command = Serial.readStringUntil('\n');
  int chosen_command = choiceMovement(command);
  for (int i=0; i<stancePosition[chosen_command].nb_mov*nb_motor; i+=nb_motor){
    setGoal(stancePosition[chosen_command].pos_motor,i);
    check_pos_achieved(stancePosition[chosen_command].pos_motor);
  }
  
  check_pos_achieved(stancePosition[chosen_command].pos_motor);
  }



int choiceMovement(String command){
  for (int i=0;i<nb_movement;i++){
    if (command == stancePosition[i].stance_name){
      return i;
    } 
  }
  return 0;
}

void setGoal(float *pos_motor, int pos){
  int mot=0;
  for(int i=pos;i<pos+nb_motor;i++){
    dxl.setGoalPosition(DXL_ID[mot], pos_motor[i], UNIT_DEGREE);
    mot++;
  }
}

void check_pos_achieved(float *target){
  bool ready = false;
  float interval = 2.0;
  while (ready == false){
    int total = 0;
    for (int i=0;i<nb_motor;i++){
      float pos_motor = dxl.getPresentPosition(DXL_ID[i], UNIT_DEGREE);
      if (abs(target[i]-pos_motor)<interval){
        total += 1;
      }
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
  setGoal(stancePosition[0].pos_motor,0);
  check_pos_achieved(stancePosition[0].pos_motor);
}

void setSpeed(uint8_t id, float speedPct) {
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
