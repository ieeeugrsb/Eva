
#include "HCSR04_Multiple_Sensors.h"

int TRIG[] = {  //Define triger pines
  30,32};
int ECHO[] = {  //Define echo pines
  31,33};
int const num_sensors = sizeof(TRIG)/sizeof(TRIG[0]);  /*The number of sensors is the number of bytes of
the array divided by the size in bytes of the fist element*/
int distance[num_sensors];  //Distance array of the same size of triger

HCSR04 ultrasonidos(TRIG, ECHO, num_sensors);


void setup() {
  Serial.begin(9600);
  Serial.println("HCSR04_v0.160215 library testing...");
  delay(1000);
  ultrasonidos.Test(TRIG, ECHO);
}

void loop() {
  Serial.println("=================");
  ultrasonidos.Reading(distance);
  for (int i = 0; i < num_sensors; i++){
    Serial.print("Distance_");
    Serial.print(i);
    Serial.print(" = ");
    Serial.print(distance[i]);
    Serial.println(" cm");
    delay(1000);
    
  }
  delay(5000);
}
