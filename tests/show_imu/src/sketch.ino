/*
  Drone Control Software - Get all the IMU data.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  The program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>

// Assign a unique ID to the sensors.
Adafruit_9DOF                 dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);

// Update this with the correct SLP for accurate altitude measurements
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

void initSensors()
{
  if(!accel.begin()) {
    // There was a problem detecting the LSM303 ... check your connections
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }

  if(!mag.begin()) {
    // There was a problem detecting the LSM303 ... check your connections
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
}

void setup()
{
    // Setup PC communication.
    Serial.begin(9600);

    // Init sensors
    initSensors();
}

void showSensorData()
{
    sensors_event_t accel_event;
    sensors_event_t mag_event;
    sensors_vec_t   orientation;

    // Calculate pitch and roll from the raw accelerometer data.
    accel.getEvent(&accel_event);
    if (dof.accelGetOrientation(&accel_event, &orientation)) {
      // 'orientation' should have valid .roll and .pitch fields
      Serial.print("Roll: ");
      Serial.print(orientation.roll);
      Serial.print("; ");
      Serial.print("Pitch: ");
      Serial.print(orientation.pitch);
      Serial.print("; ");
    }

    // Calculate the heading using the magnetometer
    mag.getEvent(&mag_event);
    if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation)) {
      // 'orientation' should have valid .heading data now
      Serial.print("Heading: ");
      Serial.print(orientation.heading);
      Serial.print("; ");
    }

    Serial.println("");
    delay(1000);
}

void loop()
{
    showSensorData();
}
