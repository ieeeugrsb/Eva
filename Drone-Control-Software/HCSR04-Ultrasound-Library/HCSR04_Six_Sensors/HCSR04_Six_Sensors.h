/************************************************************************
**  HCSR04_Six_Sensors.cpp - Library for reading six HC-SR04           **
**  Ultrasound modules.                                                **
**                                                                     **
**  Created by Jesus Rodriguez Venzal, IEEE Student Branch of Granada  **
**  03/Mar/2015                                                        **
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
  int _number_sensors;

};

#endif
