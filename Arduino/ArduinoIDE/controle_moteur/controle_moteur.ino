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
#include "real_time.h"
#include "mouv_preenregistres.h"

//OpenRB does not require the DIR control pin.
#define DXL_SERIAL Serial1
#define DEBUG_SERIAL Serial
const int DXL_DIR_PIN = -1;

const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

//This namespace is required to use Control table item names
using namespace ControlTableItem;


const int nb_moteur = 4;
const int nb_movement = 3;
const uint8_t DXL_ID[nb_moteur] = {1, 20, 3, 4};


float msg_data[nb_moteur] = {0,0,0,0};


void setup() {
  // Use UART port of DYNAMIXEL Shield to debug.
  DEBUG_SERIAL.begin(115200);
  while(!DEBUG_SERIAL);

  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  
  // initialise each moteur
  for (int i=0;i<nb_moteur;i++){
    dxl.ping(DXL_ID[i]);
    dxl.torqueOff(DXL_ID[i]);
    dxl.setOperatingMode(DXL_ID[i], OP_POSITION);
    dxl.torqueOn(DXL_ID[i]);
    defVitesse(dxl,DXL_ID[i],0.5);
  }
   homing(dxl);
}



// fonction loop() pour les mouvements préenregistés
/*
void loop() {
  while (!Serial.available()) {
    int chosen_command = 0;
    chosen_command = Serial.readString().toInt();
    Serial.print(chosen_command);
    
    if (chosen_command <= 0){
      chosen_command = 0;
    }
    envoiCommande(dxl,chosen_command);
  }
}*/


// fonction loop() pour le temps réel

void loop(){
  // J'ai besoin des cibles d'angle pour les 4 moteurs et le temps entre du écriture du port série
  bool data_received = getSerialMessage(msg_data);
  if (data_received){
    changeAngle(msg_data);
    limitePosition(msg_data);
    defVitesseAng(dxl, msg_data, 0);
    defPosAng(dxl, msg_data, 0);
  }
}
