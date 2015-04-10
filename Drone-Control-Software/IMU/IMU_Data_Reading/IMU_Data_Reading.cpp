
#include "Arduino.h"
#include "IMU_Data_Reading.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_9DOF.h>


/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_9DOF                dof   = Adafruit_9DOF();

IMU_Data::IMU_Data(){
  _Delta_velocidad_x = 0.0;
  _velocidad_x = 0.0;
  _accel_x_t_2 = 0.0;
  _accel_x_t_1 = 0.0;
  _accel_x_t = 0.0;
  _time_x_t = 0.0;
  _time_x_t_1 = 0.0;
  _time_x_t_2 = 0.0;

  _Delta_velocidad_y = 0.0;
  _velocidad_y = 0.0;
  _accel_y_t_2 = 0.0;
  _accel_y_t_1 = 0.0;
  _accel_y_t = 0.0;
  _time_y_t = 0.0;
  _time_y_t_1 = 0.0;
  _time_y_t_2 = 0.0;

  _Delta_velocidad_z = 0.0;
  _velocidad_z = 0.0;
  _accel_z_t_2 = 0.0;
  _accel_z_t_1 = 0.0;
  _accel_z_t = 0.0;
  _time_z_t = 0.0;
  _time_z_t_1 = 0.0;
  _time_z_t_2 = 0.0;
};

void IMU_Data::Initialicing(){  
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }  

  sensors_event_t event; 
  accel.getEvent(&event);
};


float IMU_Data::acceleration_x(){
  sensors_event_t event; 
  accel.getEvent(&event);

  return event.acceleration.x;
}


float IMU_Data::acceleration_y(){
  sensors_event_t event; 
  accel.getEvent(&event);

  return event.acceleration.y;
}


float IMU_Data::acceleration_z(){
  sensors_event_t event; 
  accel.getEvent(&event);

  return (event.acceleration.z);
}


float IMU_Data::velocidad_x(){

  //sensors_event_t event; 
  //accel.getEvent(&event);

  //Factor de gravedad
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_vec_t   orientation;
  accel.getEvent(&accel_event);

  _accel_x_t_2 = _accel_x_t_1;
  _accel_x_t_1 = _accel_x_t;
  _accel_x_t = 0.0;
  
  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    _g_x_imu = cos((90-orientation.pitch) * 2 *3.14159 /360)*9.807;
    if (-0.01 <= (accel_event.acceleration.x - _g_x_imu) && (accel_event.acceleration.x - _g_x_imu) <= 0.01){
      _accel_x_imu = 0.0;
    }
    else{
      _accel_x_imu = (accel_event.acceleration.x - _g_x_imu);
    }
    _accel_x_t = _accel_x_imu * cos(orientation.pitch * 2 * 3.14159 /360);
    /*if (-0.3 <= _accel_x_t && _accel_x_t <= 0.3){
     _accel_x_t = 0;
     }*/
    
    /*Serial.print("Pitch = ");
    Serial.println(orientation.pitch);
    Serial.print("Medidad IMU= ");
    Serial.print(accel_event.acceleration.x);
    Serial.println(" m/s^2");
    Serial.print("Gravedad eje X IMU = ");
    Serial.print(_g_x_imu);
    Serial.println(" m/s^2");
    Serial.print("Aceleracion sin gravedad eje X IMU = ");
    Serial.print(_accel_x_imu);
    Serial.println(" m/s^2");
    Serial.print("Aceleracion eje X GLOBAL = ");
    Serial.print(_accel_x_t);
    Serial.println(" m/s^2");*/
    Serial.print("#S|IMULOG|[");
    Serial.print(String(orientation.pitch));
    Serial.print(",");
    Serial.print(String(accel_event.acceleration.x));
    Serial.print(",");
    Serial.print(String(_g_x_imu));
    Serial.print(",");
    Serial.print(String(_accel_x_imu));
    Serial.print(",");
    Serial.print(String(_accel_x_t));
    Serial.println("]#");
  }

  _time_x_t = millis();

  //Formula de Simpson para integracion numerica
  _Delta_velocidad_x =((_time_x_t - _time_x_t_2)/(6*1000)) * (_accel_x_t_2 + 4*_accel_x_t_1 + _accel_x_t);
  _velocidad_x = _velocidad_x + _Delta_velocidad_x;

  _time_x_t_2 = _time_x_t_1;
  _time_x_t_1 = millis();

  Serial.print("Velocidad X = ");
  Serial.print(_velocidad_x);
  Serial.println(" m/s");
  Serial.print("Delta_Velocidad X = ");
  Serial.print(_Delta_velocidad_x);
  Serial.println(" m/s");

  return _velocidad_x;
}


float IMU_Data::velocidad_y(){

  sensors_event_t event; 
  accel.getEvent(&event);

  _accel_y_t_2 = _accel_y_t_1;
  _accel_y_t_1 = _accel_y_t;
  ;
  _accel_y_t = event.acceleration.y;

  _time_y_t = millis();

  //Formula de Simpson para integracion numerica
  _Delta_velocidad_y =((_time_y_t - _time_y_t_2)/(6*1000)) * (_accel_y_t_2 + 4*_accel_y_t_1 + _accel_y_t);
  _velocidad_y = _velocidad_y + _Delta_velocidad_y;

  _time_y_t_2 = _time_y_t_1;
  _time_y_t_1 = millis();

  Serial.print("Velocidad Y = ");
  Serial.print(_velocidad_y);
  Serial.println(" m/s");
  Serial.print("Delta_Velocidad Y = ");
  Serial.print(_Delta_velocidad_y);
  Serial.println(" m/s");

  return _velocidad_y;
}


float IMU_Data::velocidad_z(){

  sensors_event_t event; 
  accel.getEvent(&event);

  _accel_z_t_2 = _accel_z_t_1;
  _accel_z_t_1 = _accel_z_t;
  ;
  _accel_z_t = event.acceleration.z;

  _time_z_t = millis();

  //Formula de Simpson para integracion numerica
  _Delta_velocidad_z =((_time_z_t - _time_z_t_2)/(6*1000)) * (_accel_z_t_2 + 4*_accel_z_t_1 + _accel_z_t);
  _velocidad_z = _velocidad_z + _Delta_velocidad_z;

  _time_z_t_2 = _time_z_t_1;
  _time_z_t_1 = millis();

  Serial.print("Velocidad Z = ");
  Serial.print(_velocidad_z);
  Serial.println(" m/s");
  Serial.print("Delta_Velocidad Z = ");
  Serial.print(_Delta_velocidad_z);
  Serial.println(" m/s");

  return _velocidad_z;
}
