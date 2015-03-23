
#include "HCSR04_Six_Sensors.h"

int TRIG[] = {  //Define triger pines
  30, 32, 34, 36, 38, 40
};
int ECHO[] = {  //Define echo pines
  31, 33, 35, 37, 39, 41
};
int const num_sensors = 6;  //Number of sensores used
//int distance[num_sensors];  //Distance array of the same size of triger

HCSR04 ultrasonidos(TRIG, ECHO);

void setup() {
  Serial.begin(9600);
  Serial.println("HCSR04_Six_Sensors library testing...");
  delay(1000);
  ultrasonidos.Test(TRIG, ECHO);
}

void loop() {
  Serial.println("=================");
  ultrasonidos.Reading();
  
  Serial.print("TOP_distance"); Serial.print(" = "); Serial.println(ultrasonidos.TOP);
  Serial.print("BOTTOM_distance"); Serial.print(" = "); Serial.println(ultrasonidos.BOTTOM);
  Serial.print("FRONT_distance"); Serial.print(" = "); Serial.println(ultrasonidos.FRONT);
  Serial.print("REAR_distance"); Serial.print(" = "); Serial.println(ultrasonidos.REAR);
  Serial.print("LEFT_distance"); Serial.print(" = "); Serial.println(ultrasonidos.LEFT);
  Serial.print("RIGHT_distance"); Serial.print(" = "); Serial.println(ultrasonidos.RIGHT);

  delay(5000);
}
