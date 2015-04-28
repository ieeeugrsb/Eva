#ifndef IMU_DATA_READING_H_INCLUDED
#define IMU_DATA_READING_H_INCLUDED

#include "Arduino.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

class IMU_Data
{
  public:
    IMU_Data();

    void Initialicing();
    float pitch();
    float roll();
    float heading();
    float speedX();
    float speedY();
    float speedZ();
    float acceleration_x();
    float acceleration_y();
    float acceleration_z();

    sensors_event_t accel_event;
    sensors_event_t mag_event;
    sensors_vec_t   orientation;

  private:

    float lastPitch;
    float lastRoll;
    float lastHeading;

    float accel_Ximu;
    float gravity_Ximu;
    float accel_Yimu;
    float gravity_Yimu;
    float accel_Zimu;
    float gravity_Zimu;

    float deltaSpeed_X;
    float speed_x;
    float accelX_k2;
    float accelX_k1;
    float accelX_k;
    float timeX_k;
    float timeX_k1;
    float timeX_k2;

    float deltaSpeed_Y;
    float speed_y;
    float accelY_k2;
    float accelY_k1;
    float accelY_k;
    float timeY_k;
    float timeY_k1;
    float timeY_k2;

    float deltaSpeed_Z;
    float speed_z;
    float accelZ_k2;
    float accelZ_k1;
    float accelZ_k;
    float accel_Zreal;
    float timeZ_k;
    float timeZ_k1;
    float timeZ_k2;
};

#endif // IMU_DATA_READING_H_INCLUDED

