/*#ifndef TEMSP_REEL_H
#define TEMSP_REEL_H
#include <Arduino.h>
#include <Dynamixel2Arduino.h>
#include "serialCommunication.h"
#include "real_time.h"
 
//OpenRB does not require the DIR control pin.
#define DXL_SERIAL Serial1
#define DEBUG_SERIAL Serial
const int DXL_DIR_PIN = -1;

const float DXL_PROTOCOL_VERSION = 2.0;

//This namespace is required to use Control table item names
using namespace ControlTableItem;

class temps_reel{
  public:
  temps_reel(int* id, float* min_pos, float* max_pos, float* pos_home);
  ~temps_reel();
  void limitePosition(float *cibles_angle);
  float calculVitesse(int moteur, float cibles);
  void defVitesseAng(float *cibles);
  void defPosAng(float *cibles);
  void changeAngle(float* cibles);
  void defVitesse(uint8_t id, float pct_vitesse);
  void homing();

  Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
  const int nb_moteur = 3;
  const uint8_t DXL_ID[nb_moteur];
  float min_pos_moteur[nb_moteur];
  float max_pos_moteur[nb_moteur];
};



#endif*/
