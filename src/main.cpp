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
const int nb_motor = 2;
const uint8_t DXL_ID[nb_motor] = {1, 20}; //Motor ID
float home_value[nb_motor] = {0,0}; 
//const uint8_t DXL_ID_2 = 20;
const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
String command;

//This namespace is required to use Control table item names
using namespace ControlTableItem;

// put function declarations here:
void setSpeed(uint8_t id, float speedPct);
void homing();


void setup() {
  // put your setup code here, to run once:
  
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
  dxl.torqueOff(DXL_ID[0]);
  dxl.torqueOff(DXL_ID[1]);
  dxl.setOperatingMode(DXL_ID[0], OP_POSITION);
  dxl.setOperatingMode(DXL_ID[1], OP_POSITION);
  dxl.torqueOn(DXL_ID[0]);
  dxl.torqueOn(DXL_ID[1]);

  // Limit the maximum velocity in Position Control Mode. Use 0 for Max speed
  //dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID_1, 30);
  //dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID_2, 30);
  setSpeed(DXL_ID[0],0.2);
  setSpeed(DXL_ID[1],0.2);
}

void loop() {

  float target_1 = 90;
  float target_2 = 180;
  dxl.setGoalPosition(DXL_ID[0], target_1, UNIT_DEGREE);

  int i_present_position = 0;
  float f_present_position = 0;


  i_present_position = dxl.getPresentPosition(DXL_ID[0], UNIT_DEGREE);
  DEBUG_SERIAL.print("Present_Position 1 (degree) : ");
  DEBUG_SERIAL.println(i_present_position);
 

  // Set Goal Position in DEGREE value
  dxl.setGoalPosition(DXL_ID[1], target_2, UNIT_DEGREE);
  

  f_present_position = dxl.getPresentPosition(DXL_ID[1], UNIT_DEGREE);
  DEBUG_SERIAL.print("Present_Position 2 (degree) : ");
  DEBUG_SERIAL.println(f_present_position);

}



// put function definitions here:
void setSpeed(uint8_t id, float speedPct) {
  double maxDynamixelSpeed = 1023*0.229; //RPM
  uint32_t newSpeedRpm = speedPct*maxDynamixelSpeed;
  uint32_t writeTimeout = 100; //ms
  dxl.writeControlTableItem(PROFILE_VELOCITY, id, newSpeedRpm, writeTimeout);
}

void choiceMovement(){
  /*
  command = String(readCommand);

  if (command == "curl") {
    // do something
  }
  else if (command == "jab") {
    // do something
  }
  
  */
   
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

