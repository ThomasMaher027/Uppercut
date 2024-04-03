#include <Arduino.h>
#include "serialCommunication.h"
boolean newData = false;
const byte numChars = 64;
char receivedChars[numChars];
char tempChars[numChars];        // tableau temporaire utilisé quand on passe à travers les données
const int nb_data = 4;
 
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

void parseData(float* msg) {      // sépare les données en partie

    char * strtokIndx; // utilisé par strtok() comme un index

    strtokIndx = strtok(tempChars, ",");
    msg[0] = atof(strtokIndx);  
    
    for(int ii = 1;ii<nb_data;ii++){
        strtokIndx = strtok(NULL, ",");
        msg[ii] = atof(strtokIndx);
    }
}

//============

void showParsedData(float* msg) {
  Serial.print("OUT : <");
    for(int ii=0;ii<nb_data;ii++) {
            Serial.print(msg[ii]);
            Serial.print(", ");
    }
    Serial.println(">");
}

//============

bool getSerialMessage(float* msg){
    bool data_received = 0;
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // Copie nécessaire pour protéger les données originales,
            // à cause que strtok() utilisé dans parseData() remplace les virgules par \0 
        parseData(msg);
        //showParsedData(msg);
        newData = false;
        data_received = 1;
    }
    return data_received;
}
