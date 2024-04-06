//#include <Arduino.h>
#include "real_time.h"
//#include <Dynamixel2Arduino.h>
using namespace ControlTableItem;

 
const int nb_moteur = 4;
const uint8_t DXL_ID[nb_moteur] = {1, 20, 3, 4}; //moteur ID
float min_pos_moteur[nb_moteur] = {115, 120, 180, 45};
float max_pos_moteur[nb_moteur] = {278, 258, 270, 235};
float pos_home[nb_moteur] = {190.0, 258.0, 180.0, 235.0};
float vit_max = 0.8;
float vit_min = 0;


void changeAngle(float* cibles){
  cibles[0] = 360 - cibles[0];
  cibles[1] = 180 - cibles[1];
  cibles[2] = 180 - cibles[2];
  cibles[0] = map(cibles[0],270,360,min_pos_moteur[0], 190);
  cibles[1] = map(cibles[1],0,90,(max_pos_moteur[1]-90), max_pos_moteur[1]);
  cibles[2] = map(cibles[2],0,180,min_pos_moteur[2], max_pos_moteur[2]);
}

void limitePosition(float *cibles){
  for(int i=0;i<(nb_moteur-1);i++){
    if (cibles[i] < min_pos_moteur[i]){
      cibles[i] = min_pos_moteur[i];
    }
    if (cibles[i] > max_pos_moteur[i]){
      cibles[i] = max_pos_moteur[i];
    }
  }
    if (cibles[3] >= 1){
    cibles[3] = min_pos_moteur[3];
  }
  else{
    cibles[3] = max_pos_moteur[3];
  }
}

void defVitesseAng(Dynamixel2Arduino dxl, float *cibles, int ind){
  float erreur_pos[(nb_moteur-1)] = {0,0,0};
  float max_erreur = 0;
  for(int i=0;i<(nb_moteur-1);i++){
    float pos_actuelle = dxl.getPresentPosition(DXL_ID[i], UNIT_DEGREE);
    erreur_pos[i] = abs(cibles[i+ind] - pos_actuelle);
    if (erreur_pos[i] > max_erreur) {
      max_erreur = erreur_pos[i];
    }
  }
  float gain = 0.9/max_erreur;
  for(int i=0;i<(nb_moteur-1);i++){
    float pct_vitesse = erreur_pos[i]*gain;
    defVitesse(dxl, DXL_ID[i], pct_vitesse);
  }
}


void defPosAng(Dynamixel2Arduino dxl, float *cibles, int ind){
  for (int i=0;i<nb_moteur;i++){
    dxl.setGoalPosition(DXL_ID[i], cibles[i+ind], UNIT_DEGREE);
  }
}


void defVitesse(Dynamixel2Arduino dxl, uint8_t id, float pct_vitesse) {
  if (pct_vitesse > vit_max){
    pct_vitesse = vit_max;
  }
  if (pct_vitesse < vit_min){
    pct_vitesse = vit_min;
  }
  double vitesse_max_Dynamixel = 1023*0.229; //RPM
  uint32_t nouv_vitesse_rpm = pct_vitesse*vitesse_max_Dynamixel;
  uint32_t writeTimeout = 100; //ms
  dxl.writeControlTableItem(PROFILE_VELOCITY, id, nouv_vitesse_rpm, writeTimeout);
}

void homing(Dynamixel2Arduino dxl){
  defPosAng(dxl, pos_home, 0);
}
