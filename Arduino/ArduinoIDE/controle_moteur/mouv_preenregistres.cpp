#include "mouv_preenregistres.h"
using namespace ControlTableItem;

const int nb_moteur = 4;
const uint8_t DXL_ID[nb_moteur] = {1, 20, 3, 4}; //moteur ID


const int nb_movement = 6;
int nb_cibles = 16;
struct structStance{ 
  int stance_id;
  int nb_sub_movement;
  float cibles[16]; // moteur position (in degree)
};

struct structStance stancePosition[nb_movement] = {
  // Home
  
  {0, 1 ,{121.0, 255.0, 283.0, 230.0,
         121.0, 255.0, 283.0, 230.0,
          121.0, 255.0, 283.0, 230.0,
         121.0, 255.0, 283.0, 130.0}},

  // Curl
  {1, 1, {110.0, 140.0, 325.0, 230.0,
          150.0, 140.0, 280.0, 230.0,
          150.0, 140.0, 280.0, 230.0,
          150.0, 140.0, 280.0, 230.0}},

  // Jab
  {2, 3, {185.0, 167.0, 291.0, 230.0,
          95.0, 164.0, 193.0, 230.0,
          185.0, 167.0, 291.0, 230.0,
          185.0, 167.0, 291.0, 230.0}},

    // Uppercut
  {3, 3, {162.0, 256.0, 265.0, 2300.0,
          107.0, 255.0, 235.0, 230.0,
          162.0, 256.0, 265.0, 230.0,
          162.0, 256.0, 265.0, 230.0}},

   // Crochet
  {4, 3, {182.0, 168.0, 227.0, 230.0,
          110.0, 166.0, 247.0, 230.0,
          167.0, 256.0, 289.0, 230.0,
           167.0, 256.0, 289.0, 230.0}},
    // Overhead
   {5, 2, {191.0, 110.0, 224.0, 230.0,
           150.0, 110.0, 252.0, 230.0,
           99.0, 110.0, 252.0, 230.0,
           99.0, 110.0, 252.0, 230.0}},
};


// S'assure que tous les moteurs ont atteint
void check_pos_achieved(Dynamixel2Arduino dxl, float *cibles, int j){
  bool ready = false;
  float interval = 8.0; //Le moteur doit être à la position cible +/- interval
  while (ready == false){
    int total = 0;
    int cibles_index = j;
    for (int i=0;i<nb_moteur-1;i++){
      float pos_moteur = dxl.getPresentPosition(DXL_ID[i], UNIT_DEGREE);
      if (abs(cibles[cibles_index]-pos_moteur)<interval){
        total += 1;
      }
      cibles_index += 1;
    }
    if (total == nb_moteur-1){
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
