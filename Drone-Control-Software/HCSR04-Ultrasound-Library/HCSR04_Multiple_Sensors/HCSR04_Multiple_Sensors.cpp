/************************************************************************
**  HCSR04_Multiple_Sensors.cpp - Library for reading multiple HC-SR04 **
**  Ultrasound modules.                                                **
**                                                                     **
**  Created by Jesus Rodriguez Venzal, IEEE Student Branch of Granada  **
**  19/Feb/2015                                                        **
************************************************************************/

#include "Arduino.h"
#include "HCSR04_Multiple_Sensors.h"

HCSR04::HCSR04(int trig_in[],int echo_in[],int num_sensors){
/**********************************************************************
**  When defining an object of this class it will storage in private **
**  variables triger and echo pins vectors, defining each triger pin **
**  as OUTPUT and each echo pin as INPUT. They are also set to LOW   **
**  state.                                                           **
**********************************************************************/

  number_sensors = num_sensors;  //Load number of sensors used
  triger = new int[number_sensors];
  echo = new int[number_sensors];  /*Create triger and echo arrays with the length of the number of sensors
  and load the address of the first element in the pointer*/

  for (int i = 0; i < number_sensors; i++){
    triger[i] = trig_in[i];  //Load triger pins
    pinMode(triger[i], OUTPUT);  //Define them as OUTPUT
    digitalWrite(triger[i], LOW);  //Set them as LOW to avoid problems
    echo[i] = echo_in[i];  //Load echo pins
    pinMode(echo[i], INPUT);  //Define them as INPUT
    digitalWrite(echo[i], LOW);  //Set them as LOW to avoid problems
  }
}

void HCSR04::Reading(int distancia[]){
/************************************************************************
**  This function send an ultrasonic pulse with every sensor reading   **
**  the duration of the echo received. After a triger 10 us HIGH       **
**  signal the sensor send out an 8 cycle burst of ultrasounds and     **
**  rise its echo pin, when they are recived echo is set as LOW again  **
**  The formula to calculate the distances is:                         **
**            Distance = High_Level_Time * Sound_speed / 2             **
**  (divided by 2 because ultrasounds have to go and return)           **
************************************************************************/

  //Set all the sensors as LOW during 2 us to stabilize them
  for (int i = 0; i < number_sensors; i++){
    digitalWrite(triger[i], LOW);
  }
  delayMicroseconds(2);
  //Send 10 us start ranging signal
  for (int i = 0; i < number_sensors; i++){
    digitalWrite(triger[i], HIGH);
  }
  delayMicroseconds(10);
  for (int i = 0; i < number_sensors; i++){
    digitalWrite(triger[i], LOW);
    distancia[i] = pulseIn(echo[i], HIGH) /29 / 2 ;  //Distance in cm (1/29 = 0.034 cm/us)
  }
}

void HCSR04::Test(int trig_outside[], int echo_outside[]){
/***********************************************************************
**  This function verifies if triger and echo inside the obsject are  **
**  the same as those defined in the main sketch                      **
***********************************************************************/

  Serial.println("Testing allocation of triger and echo pins:");
  for (int i = 0; i < number_sensors; i++){
    if (trig_outside[i] != triger[i]){
      Serial.print("Error in triger allocation: Triger pin ");
      Serial.print(trig_outside[i]);
      Serial.println(" is corrupt inside the class");
    }
    else if (echo_outside[i] != echo[i]){
      Serial.print("Error in echo allocation: Echo pin ");
      Serial.print(echo_outside[i]);
      Serial.println(" is corrupt inside the class");
    }
    else{
      Serial.print("Pin ");
      Serial.print(trig_outside[i]);
      Serial.print(" & ");
      Serial.print(echo_outside[i]);
      Serial.println(" allocation is correct");
    }
  }
}
