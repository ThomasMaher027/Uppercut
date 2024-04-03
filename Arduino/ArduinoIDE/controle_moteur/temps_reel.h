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
  temps_reel(int* id, float* min_pos, float* max_pos, float* home_pos);
  ~temps_reel();
  void limitPosition(float *target_angle);
  float getSpeed(int motor, float target);
  void setAngularSpeed(float *target);
  void setAngularPosition(float *target);
  void changeAngle(float* target);
  void set_speed(uint8_t id, float speedPct);
  void homing();

  Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
  const int nb_motor = 3;
  const uint8_t DXL_ID[nb_motor];
  float min_pos_motor[nb_motor];
  float max_pos_motor[nb_motor];
};



#endif*/
