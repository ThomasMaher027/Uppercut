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
    target_pos1 = atof(strtokIndx);  
    
    strtokIndx = strtok(NULL, ",");
    target_pos2 = atof(strtokIndx);
    
    strtokIndx = strtok(NULL, ",");
    target_pos3 = atof(strtokIndx);
    
    strtokIndx = strtok(NULL, ",");
    target_pos4 = atof(strtokIndx);
    
    strtokIndx = strtok(NULL, ",");
    delta_time = atof(strtokIndx);

}

//============

void showParsedData() {
    Serial.print("Target 1 : ");
    Serial.println(target_pos1);
    
    Serial.print("Target 2 : ");
    Serial.println(target_pos2);
    
    Serial.print("Target 3 : ");
    Serial.println(target_pos3);
    
    Serial.print("Target 4 : ");
    Serial.println(target_pos4);
    
    Serial.print("Delta temps : ");
    Serial.println(delta_time);
}

//============

void getSerialMessage(){
      recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // Copie nécessaire pour protéger les données originales,
            // à cause que strtok() utilisé dans parseData() remplace les virgules par \0 
        parseData();
        showParsedData();
        newData = false;
    }
}