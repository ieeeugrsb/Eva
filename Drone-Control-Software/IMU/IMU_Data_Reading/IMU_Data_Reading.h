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
  float velocidad_x();
  float velocidad_y();
  float velocidad_z();
  float acceleration_x();
  float acceleration_y();
  float acceleration_z();

private:
  float _Delta_velocidad_x;
  float _velocidad_x;
  float _accel_x_t_2;
  float _accel_x_t_1;
  float _accel_x_t;
  float _time_x_t;
  float _time_x_t_1;
  float _time_x_t_2;
  
  float _accel_x_imu;
  float _g_x_imu;
  
  float _Delta_velocidad_y;
  float _velocidad_y;
  float _accel_y_t_2;
  float _accel_y_t_1;
  float _accel_y_t;
  float _time_y_t;
  float _time_y_t_1;
  float _time_y_t_2;
  
  float _Delta_velocidad_z;
  float _velocidad_z;
  float _accel_z_t_2;
  float _accel_z_t_1;
  float _accel_z_t;
  float _time_z_t;
  float _time_z_t_1;
  float _time_z_t_2;
};

#endif // IMU_DATA_READING_H_INCLUDED
