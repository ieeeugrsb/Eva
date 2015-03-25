/************************************************************************
**  HCSR04_Six_Sensors.cpp - Library for reading six HC-SR04           **
**  Ultrasound modules.                                                **
**                                                                     **
**  Created by Jesus Rodriguez Venzal, IEEE Student Branch of Granada  **
**  17/Mar/2015                                                        **
************************************************************************/

#include "Arduino.h"
#include "HCSR04_Six_Sensors.h"

HCSR04::HCSR04(int trig_in[],int echo_in[]){
/**********************************************************************
**  When defining an object of this class it will storage in private **
**  variables triger and echo pins vectors, defining each triger pin **
**  as OUTPUT and each echo pin as INPUT. They are also set to LOW   **
**  state.                                                           **
**********************************************************************/

  _number_sensors = 6;  //Load number of sensors used
  _triger = new int[_number_sensors];
  _echo = new int[_number_sensors];  /*Create triger and echo arrays with the length of the number of sensors
  and load the address of the first element in the pointer*/

  for (int i = 0; i < _number_sensors; i++){
    _triger[i] = trig_in[i];  //Load triger pins
    pinMode(_triger[i], OUTPUT);  //Define them as OUTPUT
    digitalWrite(_triger[i], LOW);  //Set them as LOW to avoid problems
    _echo[i] = echo_in[i];  //Load echo pins
    pinMode(_echo[i], INPUT);  //Define them as INPUT
    digitalWrite(_echo[i], LOW);  //Set them as LOW to avoid problems
  }
}

void HCSR04::Reading(){
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
  for (int i = 0; i < _number_sensors; i++){
    digitalWrite(_triger[i], LOW);
  }
  delayMicroseconds(2);
  //Send 10 us start ranging signal
  for (int i = 0; i < _number_sensors; i++){
    digitalWrite(_triger[i], HIGH);
  }
  delayMicroseconds(10);
  for (int i = 0; i < _number_sensors; i++){
    digitalWrite(_triger[i], LOW);
    _distance[i] = pulseIn(_echo[i], HIGH) /29 / 2 ;  //Distance in cm (1/29 = 0.034 cm/us)
  }
  TOP = _distance[0];
  BOTTOM = _distance[1];
  FRONT = _distance[2];
  REAR = _distance[3];
  LEFT = _distance[4];
  RIGHT = _distance[5];
}

void HCSR04::Test(int trig_outside[], int echo_outside[]){
/***********************************************************************
**  This function verifies if triger and echo inside the obsject are  **
**  the same as those defined in the main sketch                      **
***********************************************************************/

  Serial.println("Testing allocation of triger and echo pins:");
  for (int i = 0; i < _number_sensors; i++){
    if (trig_outside[i] != _triger[i]){
      Serial.print("Error in triger allocation: Triger pin ");
      Serial.print(trig_outside[i]);
      Serial.println(" is corrupt inside the class");
    }
    else if (echo_outside[i] != _echo[i]){
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
