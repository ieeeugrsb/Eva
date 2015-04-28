
#include <adafruit_9dof.h>
#include "IMU_Data.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>

IMU_Data IMU;

void setup() 
{
  Serial.begin(115200);
  Serial.println("Accelerometer Test"); 
  Serial.println("");
  
  IMU.Initialicing();

}

void loop() 
{
  Serial.println("BEGINING TEST");
  Serial.println("=============================");
  Serial.print("Pitch ");
  Serial.print(IMU.pitch());
  Serial.print("  Roll ");
  Serial.print(IMU.roll());
  Serial.print("  Heading ");
  Serial.println(IMU.heading());
  Serial.print("AccelerationX ");
  Serial.print(IMU.acceleration_x()); 
  Serial.print(" m^2/s  AccelerationY ");
  Serial.print(IMU.acceleration_x()); 
  Serial.print(" m^2/s  AccelerationZ ");
  Serial.print(IMU.acceleration_z()); 
  Serial.println(" m^2/s");
  Serial.print("SpeedX ");
  Serial.print(IMU.speedX());
  Serial.print(" m/s  SpeedY ");
  Serial.print(IMU.speedY());
  Serial.print(" m/s  SpeedZ ");
  Serial.print(IMU.speedZ());
  Serial.println(" m/s");
  
  delay(2000);
}
