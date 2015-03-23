/************************************************************************
**  HCSR04_Six_Sensors.cpp - Library for reading six HC-SR04           **
**  Ultrasound modules.                                                **
**                                                                     **
**  Created by Jesus Rodriguez Venzal, IEEE Student Branch of Granada  **
**  17/Mar/2015                                                        **
************************************************************************/

#ifndef HCSR04_Six_Sensors_h
#define HCSR04_Six_Sensors_h

#include "Arduino.h"

class HCSR04
{
public:

  HCSR04(int trig_in[],int echo_in[]);
  void Reading();
  void Test(int trig_outside[], int echo_outside[]);
  int TOP;
  int BOTTOM;
  int LEFT;
  int RIGHT;
  int REAR;
  int FRONT;

private:

  int *_triger;
  int *_echo;
  int _number_sensors;
  int _distance[6];

};

#endif
