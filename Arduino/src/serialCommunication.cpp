// Code fait par Robin2 à : https://forum.arduino.cc/t/serial-input-basics-updated/382007/3

#include <Arduino.h>
#include "serialCommunication.h"

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // tableau temporaire utilisé quand on passe à travers les données

      // variables pour conserver les données
float target_pos1 = 0.0;
float target_pos2 = 0.0;
float target_pos3 = 0.0;
float target_pos4 = 0.0;
float delta_time = 0.0;

const int nb_data = 4;
float msg[nb_data] = {0, 0, 0, 0};


boolean newData = false;


//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // termine la chaine de caractère
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // sépare les données en partie

    char * strtokIndx; // utilisé par strtok() comme un index

    strtokIndx = strtok(tempChars, ",");
    msg[0] = atof(strtokIndx);  
    
    for(int ii = 1;ii<nb_data;ii++){
        strtokIndx = strtok(NULL, ",");
        msg[ii] = atof(strtokIndx);
    }
}

//============

void showParsedData() {
    for(int ii=0;ii<nb_data;ii++) {
        if(ii<(nb_data-1)) {
            Serial.print("Target ");
            Serial.print(ii);
            Serial.print(" : ");

            Serial.println(msg[ii]);
        }
        else {
            Serial.print("Delta temps : ");
            Serial.println(msg[ii]);
        }

    }
}

//============

float* getSerialMessage(){
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // Copie nécessaire pour protéger les données originales,
            // à cause que strtok() utilisé dans parseData() remplace les virgules par \0 
        parseData();
        showParsedData();
        newData = false;
        return msg;
    }
}