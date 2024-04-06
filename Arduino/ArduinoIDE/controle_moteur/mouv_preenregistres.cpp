#include "mouv_preenregistres.h"
using namespace ControlTableItem;

const int nb_moteur = 4;
const uint8_t DXL_ID[nb_moteur] = {1, 20, 3, 4}; //moteur ID


const int nb_movement = 3;
int nb_cibles = 12;
struct structStance{ 
  int stance_id;
  int nb_sub_movement;
  float cibles[12]; // moteur position (in degree)
};

struct structStance stancePosition[nb_movement] = {
  // Home
  
  {0, 1 ,{190.0, 258.0, 70.0,
          200.0, 140.0, 150.0,
          200.0, 140.0, 150.0,
          200.0, 140.0, 150.0}},

  // Curl
  {1, 1, {110.0, 140.0, 325.0, 
          150.0, 140.0, 280.0, 
          150.0, 140.0, 280.0, 
          150.0, 140.0, 280.0}},

  // Jab
  {2, 3, {196.0, 50.0, 305.0, 
          110.4, 50.0, 178.5, 
          196.0, 50.0, 305.0,
          196.0, 50.0, 305.0}},
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
};

void check_pos_achieved(Dynamixel2Arduino dxl, float *cibles, int j){
  bool ready = false;
  float interval = 2.0;
  while (ready == false){
    int total = 0;
    int cibles_index = j;
    for (int i=0;i<nb_moteur;i++){
      float pos_moteur = dxl.getPresentPosition(DXL_ID[i], UNIT_DEGREE);
      if (abs(cibles[cibles_index]-pos_moteur)<interval){
        total += 1;
      }
      cibles_index += 1;
    }
    if (total == nb_moteur){
      ready = true;
    }
    else if (total > nb_moteur){
      Serial.println("Le check pour savoir si les moteurs sont arrives a destination n'est pas bien fait");
    }
  }
}

void envoiCommande(Dynamixel2Arduino dxl, int chosen_command){
  if (chosen_command < nb_movement){
      for (int i=0; i<stancePosition[chosen_command].nb_sub_movement*nb_moteur; i+=nb_moteur){
          defVitesseAng(dxl, stancePosition[chosen_command].cibles, i);
          defPosAng(dxl, stancePosition[chosen_command].cibles, i);
          check_pos_achieved(dxl, stancePosition[chosen_command].cibles, i);
      homing(dxl);
    }
  }
}


/*
void setGoal_Position(Dynamixel2Arduino dxl, float *cibles, int pos){
  int moteur=0;
  for(int i=pos;i<pos+nb_moteur;i++){
    float pct_vitesse = calculVitesse(dxl, moteur, cibles[i]);
    defVitesse(dxl, DXL_ID[moteur], pct_vitesse);
    dxl.setGoalPosition(DXL_ID[moteur], cibles[i], UNIT_DEGREE);
    moteur++;
  }
}*/
/*
float calculVitesse(Dynamixel2Arduino dxl, int moteur, float cibles){
  // Égal à 1/220. Donc, pour une erreur d'angle de 220 deg, la vitesse est 100% (les moteurs ne tourneront pas plus de 270 deg) 
  float p_gain = 0.004545455;
  float error = abs(dxl.getPresentPosition(DXL_ID[moteur], UNIT_DEGREE) - cibles);
  float out_speed = error*p_gain;
  return out_speed; 
}*/
/*
void setGoalSpeed(Dynamixel2Arduino dxl, float *cibles, int pos){
  int moteur=0;
  for(int i=pos;i<pos+nb_moteur;i++){
    defVitesse(dxl, DXL_ID[moteur], cibles[i]);
    moteur++;
  }
}*/



/*
void printPosition(Dynamixel2Arduino dxl){
  for(int i=0;i<nb_moteur;i++){
    float pos_actuelleition = dxl.getPresentPosition(DXL_ID[i], UNIT_DEGREE);
    DEBUG_SERIAL.print("pos_actuelleition ");
    DEBUG_SERIAL.print(i);
    DEBUG_SERIAL.print(" (degree) : ");
    DEBUG_SERIAL.println(pos_actuelleition);
  }
}
*/
/*
int choiceMovement(int command){
  for (int i=0;i<nb_movement;i++){
    if (command == stancePosition[i].stance_id){
      return i;
    } 
  }
  return -1;
}*/
