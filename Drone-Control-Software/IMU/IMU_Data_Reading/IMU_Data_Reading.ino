
#include <adafruit_9dof.h>
#include "IMU_Data_Reading.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>

/* Assign a unique ID to this sensor at the same time 
 Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);*/

IMU_Data IMU;

float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

void setup() 
{
  Serial.begin(115200);
  Serial.println("Accelerometer Test"); 
  Serial.println("");
  Serial.println("#S|CPIMULOG|[]#");
  Serial.println("#S|IMULOG|[Accelerometer Test]#");
  Serial.println("#S|IMULOG|[Pitch,Medida(m/s^2),Gx,Ax_IMU,Ax_GLOBAL]#");
  
  IMU.Initialicing();

}

void loop() 
{
  Serial.println("BEGINING TEST");
  Serial.println("=============================");
  Serial.print(IMU.velocidad_x()); 
  Serial.println(" m/s");
  delay(2000);
}
