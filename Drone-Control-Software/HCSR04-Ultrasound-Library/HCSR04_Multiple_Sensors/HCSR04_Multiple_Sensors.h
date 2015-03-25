/************************************************************************
**  HCSR04_Multiple_Sensors.cpp - Library for reading multiple HC-SR04 **
**  Ultrasound modules.                                                **
**                                                                     **
**  Created by Jesus Rodriguez Venzal, IEEE Student Branch of Granada  **
**  19/Feb/2015                                                        **
************************************************************************/

#ifndef HCSR04_Multiple_Sensors_h
#define HCSR04_Multiple_Sensors_h

#include "Arduino.h"

class HCSR04
{
public:

  HCSR04(int trig_in[],int echo_in[],int num_sensores);
  void Reading(int distance[]);
  void Test(int trig_outside[], int echo_outside[]);

private:

  int *triger;
  int *echo;
  int number_sensors;

};

#endif
