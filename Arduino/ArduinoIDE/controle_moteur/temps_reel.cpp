/*#include "temps_reel.h"



temps_reel::temps_reel(int* id, float* min_pos, float* max_pos, float* pos_home){
  for(int ii=0;ii<nb_moteur;ii++){
      DXL_ID[ii] = id[ii];
      min_pos_moteur[ii] = min_pos[ii]; 
      max_pos_moteur[nb_moteur] = max_pos[ii];
  }
}



temps_reel::~temps_reel(){
}



void temps_reel::changeAngle(float* cibles){
  cibles[0] = 360 - cibles[0];
  cibles[1] = 180 - cibles[1];
  cibles[2] = 90 - cibles[2];
  cibles[0] = map(cibles[0],270,360,min_pos_moteur[0], 190);
  cibles[1] = map(cibles[1],0,90,(max_pos_moteur[1]-90), max_pos_moteur[1]);
  cibles[2] = map(cibles[2],0,180,min_pos_moteur[2], max_pos_moteur[2])*2;
}



void temps_reel::limitePosition(float *cibles_angle){
  for(int i=0;i<nb_moteur;i++){
    if (cibles_angle[i] < min_pos_moteur[i]){
      cibles_angle[i] = min_pos_moteur[i];
    }
    if (cibles_angle[i] > max_pos_moteur[i]){
      cibles_angle[i] = max_pos_moteur[i];
    }
  }
}



void temps_reel::defVitesseAng(float *cibles){
  float erreur_pos[nb_moteur] = {0,0,0};
  float max_erreur = 0;
  for(int i=0;i<nb_moteur;i++){
    float pos_actuelle = dxl.getPresentPosition(DXL_ID[i], UNIT_DEGREE);
    erreur_pos[i] = abs(cibles[i] - pos_actuelle);
    if (erreur_pos[i] > max_erreur) {
      max_erreur = erreur_pos[i];
    }
  }
  float gain = 0.9/max_erreur;
  for(int i=0;i<nb_moteur;i++){
    float pct_vitesse = erreur_pos[i]*gain;
    defVitesse(DXL_ID[i], pct_vitesse);
  }
}



void temps_reel::defPosAng(float *cibles){
  for (int i=0;i<nb_moteur;i++){
    dxl.setGoalPosition(DXL_ID[i], cibles[i], UNIT_DEGREE);
  }
}



void temps_reel::defVitesse(uint8_t id, float pct_vitesse) {
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
}*/
