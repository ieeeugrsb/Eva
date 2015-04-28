
#include "Arduino.h"
#include "IMU_Data.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_9DOF.h>


/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_9DOF                dof   = Adafruit_9DOF();

IMU_Data::IMU_Data() {

  lastPitch = 0;
  lastRoll = 0;
  lastHeading = 0;

  deltaSpeed_X = 0.0;
  speed_x = 0.0;
  accelX_k2 = 0.0;
  accelX_k1 = 0.0;
  accelX_k = 0.0;
  timeX_k = 0.0;
  timeX_k1 = 0.0;
  timeX_k2 = 0.0;

  deltaSpeed_Y = 0.0;
  speed_y = 0.0;
  accelY_k2 = 0.0;
  accelY_k1 = 0.0;
  accelY_k = 0.0;
  timeY_k = 0.0;
  timeY_k1 = 0.0;
  timeY_k2 = 0.0;

  deltaSpeed_Z = 0.0;
  speed_z = 0.0;
  accelZ_k2 = 0.0;
  accelZ_k1 = 0.0;
  accelZ_k = 0.0;
  timeZ_k = 0.0;
  timeZ_k1 = 0.0;
  timeZ_k2 = 0.0;
};

void IMU_Data::Initialicing() {
  if (!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1);
  }

  if (!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1);
  }
};


float IMU_Data::pitch() {
  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    float _pitch = orientation.pitch;
    //  Set the maximum accuracy to 0.5 degrees to avoid continuous fluctuations
    if (lastPitch - _pitch <= 0.5 && lastPitch - _pitch >= -0.5) {
      return lastPitch;
    }
    else {
      lastPitch = _pitch;
      return _pitch;
    }
  }
}


float IMU_Data::roll() {
  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    float _roll = orientation.roll;
    //  Set the maximum accuracy to 0.5 degrees to avoid continuous fluctuations
    if (lastRoll - _roll <= 0.5 && lastRoll - _roll >= -0.5) {
      return lastRoll;
    }
    else {
      lastRoll = _roll;
      return _roll;
    }
  }
}


float IMU_Data::heading() {
  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {
    float _heading = orientation.heading;
    //  Set the maximum accuracy to 0.5 degrees to avoid continuous fluctuations
    if (lastHeading - _heading <= 0.5 && lastHeading - _heading >= -0.5) {
      return lastHeading;
    }
    else {
      lastHeading = _heading;
      return _heading;
    }
  }
}


float IMU_Data::acceleration_x() {
  accel.getEvent(&accel_event);

  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    gravity_Ximu = sin((orientation.pitch) * 3.14159 / 180) * 9.807;
    if (-0.01 <= (accel_event.acceleration.x - gravity_Ximu) && (accel_event.acceleration.x - gravity_Ximu) <= 0.01) {
      accel_Ximu = 0.0;
    }
    else {
      accel_Ximu = (accel_event.acceleration.x - gravity_Ximu);
    }
  }

  return accel_Ximu / cos(orientation.pitch * 3.14159 / 180);
}


float IMU_Data::acceleration_y() {
  accel.getEvent(&accel_event);

  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    gravity_Yimu = sin((orientation.roll) * 3.14159 / 180) * 9.807;
    if (-0.01 <= (accel_event.acceleration.y - gravity_Yimu) && (accel_event.acceleration.y - gravity_Yimu) <= 0.01) {
      accel_Yimu = 0.0;
    }
    else {
      accel_Yimu = (accel_event.acceleration.y - gravity_Yimu);
    }
  }

  return accel_Yimu / cos(orientation.roll * 3.14159 / 180);
}


float IMU_Data::acceleration_z() {
  accel.getEvent(&accel_event);

  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    //Calculate trigonometric functions of the Z axis angle
    float _sine2_z_angle = sin((orientation.roll) * 3.14159 / 180) * sin((orientation.roll) * 3.14159 / 180) + sin((orientation.pitch) * 3.14159 / 180) * sin((orientation.pitch) * 3.14159 / 180);
    float _sine_z_angle = sqrt(_sine2_z_angle);
    float _cosine2_z_angle = 1 - _sine2_z_angle;
    float _cosine_z_angle = sqrt(_cosine2_z_angle);

    gravity_Zimu = -9.9 * _cosine_z_angle;  //  Aproximation to be close to gravity measured from the IMU
    //  To avoid static error
    if (-0.20 <= (abs(accel_event.acceleration.z) - abs(gravity_Zimu)) && (abs(accel_event.acceleration.z) - abs(gravity_Zimu)) <= 0.20) {
      accel_Zimu = 0.0;
    }
    else {
      accel_Zimu = accel_event.acceleration.z - gravity_Zimu;
    }
    accel_Zreal = accel_Zimu / _cosine_z_angle;
  }
  return accel_Zreal;
}


float IMU_Data::speedX() {

  //Factor de gravedad
  accel.getEvent(&accel_event);

  accelX_k2 = accelX_k1;
  accelX_k1 = accelX_k;

  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    gravity_Ximu = sin((orientation.pitch) * 3.14159 / 180) * (- 9.807);
    if (-0.01 <= (abs(accel_event.acceleration.x) - abs(gravity_Ximu)) && (abs(accel_event.acceleration.x) - abs(gravity_Ximu)) <= 0.01) {
      accel_Ximu = 0.0;
    }
    else {
      accel_Ximu = (accel_event.acceleration.x + gravity_Ximu);
    }
    accelX_k = accel_Ximu / cos(orientation.pitch * 3.14159 / 180);
  }

  timeX_k = millis();

  //Formula de Simpson para integracion numerica
  deltaSpeed_X = ((timeX_k - timeX_k2) / (6 * 1000)) * (accelX_k2 + 4 * accelX_k1 + accelX_k);
  speed_x = speed_x + deltaSpeed_X;

  timeX_k2 = timeX_k1;
  timeX_k1 = millis();

  return speed_x;
}


float IMU_Data::speedY() {

  //Factor de gravedad
  accel.getEvent(&accel_event);

  accelY_k2 = accelY_k1;
  accelY_k1 = accelY_k;

  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    gravity_Yimu = sin((orientation.roll) * 3.14159 / 180) * (- 9.807);
    if (-0.01 <= (abs(accel_event.acceleration.y) - abs(gravity_Yimu)) && (abs(accel_event.acceleration.y) - abs(gravity_Yimu)) <= 0.01) {
      accel_Yimu = 0.0;
    }
    else {
      accel_Yimu = (accel_event.acceleration.y + gravity_Yimu);
    }
    accelY_k = accel_Yimu / cos(orientation.roll * 3.14159 / 180);
  }

  timeY_k = millis();

  //Formula de Simpson para integracion numerica
  deltaSpeed_Y = ((timeY_k - timeY_k2) / (6 * 1000)) * (accelY_k2 + 4 * accelY_k1 + accelY_k);
  speed_y = speed_y + deltaSpeed_Y;

  timeY_k2 = timeY_k1;
  timeY_k1 = millis();

  return speed_y;
}


float IMU_Data::speedZ() {

  //Factor de gravedad
  accel.getEvent(&accel_event);

  accelZ_k2 = accelZ_k1;
  accelZ_k1 = accelZ_k;

  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    //Calculate trigonometric functions of the Z axis angle
    float _sine2_z_angle = sin((orientation.roll) * 3.14159 / 180) * sin((orientation.roll) * 3.14159 / 180) + sin((orientation.pitch) * 3.14159 / 180) * sin((orientation.pitch) * 3.14159 / 180);
    float _sine_z_angle = sqrt(_sine2_z_angle);
    float _cosine2_z_angle = 1 - _sine2_z_angle;
    float _cosine_z_angle = sqrt(_cosine2_z_angle);

    gravity_Zimu = -9.9 * _cosine_z_angle;  //  Aproximation to be close to gravity measured from the IMU
    //  To avoid static error
    if (-0.20 <= (abs(accel_event.acceleration.z) - abs(gravity_Zimu)) && (abs(accel_event.acceleration.z) - abs(gravity_Zimu)) <= 0.20) {
      accel_Zimu = 0.0;
    }
    else {
      accel_Zimu = accel_event.acceleration.z - gravity_Zimu;
    }
    accelZ_k = accel_Zimu / _cosine_z_angle;
  }
  timeZ_k = millis();

  //Formula de Simpson para integracion numerica
  deltaSpeed_Z = ((timeZ_k - timeZ_k2) / (6 * 1000)) * (accelZ_k2 + 4 * accelZ_k1 + accelZ_k);
  if (abs(deltaSpeed_Z) <= 0.3) {
    deltaSpeed_Z = 0;
  }
  speed_z = speed_z + deltaSpeed_Z;

  timeZ_k2 = timeZ_k1;
  timeZ_k1 = millis();

  return speed_z;
}


