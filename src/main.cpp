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
const int nb_motor = 1;
const int nb_movement = 1;
const uint8_t DXL_ID[nb_motor] = {1}; //Motor ID
float home_value[nb_motor] = {0}; 
const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
String command;

//This namespace is required to use Control table item names
using namespace ControlTableItem;

// put function declarations here:
void setSpeed(uint8_t id, float speedPct);
void homing();
void printPosition();
int choiceMovement(String command);
void check_pos_achieved(float target[]);
void setGoal();

struct _stancePosition{ // motor position (in degree)
  char stance_name[20];
  float pos_motor[nb_motor];
};

struct _stancePosition stancePosition[nb_movement] = {
  {"curl",{80.0}},
};

void setup() {
  // Use UART port of DYNAMIXEL Shield to debug.
  DEBUG_SERIAL.begin(115200);
  while(!DEBUG_SERIAL);

  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  
  // Get DYNAMIXEL information
  dxl.ping(DXL_ID[0]);
  dxl.ping(DXL_ID[1]);

  // Turn off torque when configuring items in EEPROM area
  for (int i=0;i<nb_motor;i++){
    dxl.torqueOff(DXL_ID[i]);
    dxl.setOperatingMode(DXL_ID[i], OP_POSITION);
    dxl.torqueOn(DXL_ID[i]);
    setSpeed(DXL_ID[i],0.2);
  }
  homing();
}



void loop() {
  bool ready = false;
  String command = Serial.readStringUntil('\n');
  int chosen_command = choiceMovement(command);
    /*dxl.setGoalPosition(DXL_ID[0], curl.pos_elbow_motor, UNIT_DEGREE);
    float target[nb_motor] = {curl.pos_elbow_motor};
  check_pos_achieved(target);*/
  homing();
  }



void setSpeed(uint8_t id, float speedPct) {
  double maxDynamixelSpeed = 1023*0.229; //RPM
  uint32_t newSpeedRpm = speedPct*maxDynamixelSpeed;
  uint32_t writeTimeout = 100; //ms
  dxl.writeControlTableItem(PROFILE_VELOCITY, id, newSpeedRpm, writeTimeout);
}

int choiceMovement(String command){
  for (int i=0;i<nb_movement;i++){
    if (command == stancePosition[i].stance_name){
      return i;
    } 
  }
  return nb_movement+1; // Va faire crash le programme car on retourne un index out-of-bound (on a pas reçu une commande correspondante à un mouvement prédéfini)
}

void setGoal(){
}

void homing(){
  bool ready = false;
  for(int i=0;i<nb_motor;i++){
    dxl.setGoalPosition(DXL_ID[i], home_value[i], UNIT_DEGREE);
  } 
  while (ready == false){
    float pos_motor_0 = dxl.getPresentPosition(DXL_ID[0], UNIT_DEGREE);
    float pos_motor_1 = dxl.getPresentPosition(DXL_ID[1], UNIT_DEGREE);
    if ((abs(home_value[0]-pos_motor_0)>2.0) && (abs(home_value[1]-pos_motor_1)>2.0)){
      ready = true;
    }
  }
}

void check_pos_achieved(float target[]){
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

void printPosition(){
  for(int i=0;i<nb_motor;i++){
    float present_position = dxl.getPresentPosition(DXL_ID[i], UNIT_DEGREE);
    DEBUG_SERIAL.print("Present_Position ");
    DEBUG_SERIAL.print(i);
    DEBUG_SERIAL.print(" (degree) : ");
    DEBUG_SERIAL.println(present_position);
  }
}

