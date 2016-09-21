//Sensor_Read.ino

#include <math.h>
#include <Adafruit_9DOF.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include "IMUComplementaryFilter.h"
#include <PID_v1.h>

double roll_gyro;
double gyro_x;
double theta_acc;

double current_angle;
double acceleration_x;

double thrust = 0;
double roll_s = 0, roll = 0, roll_u=0;
double pitch_s = 0, pitch, pitch_u;
double yaw_s = 0, yaw, yaw_u;
int motor1_u=0, motor2_u=0, motor3_u=0, motor4_u=0;

// Output limits
const double roll_min=-100, roll_max=100;
const double pitch_min=-75, pitch_max=75;
const double yaw_min=-75, yaw_max=75;


unsigned long previousMillis = 0;        // will store last time a sensor read was done
const long interval = 10;           // interval at wich the sampling is done (default 10ms)

//IMUComplementaryFilter roll_filter = IMUComplementaryFilter(0.04, 0.00001,interval);

// Assign a unique ID to the sensors.
Adafruit_9DOF                 dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);


const int MOTOR_BL = 12;    // PWM pin for Back-Left Motor
const int MOTOR_BR = 11;    // PWM pin for Back-Right Motor
const int MOTOR_FL = 10;    // PWM pin for Front-Left Motor
const int MOTOR_FR =  9;    // PWM pin for Front-Right Motor

//Define tuning parameters
double roll_Kp=0.2, roll_Ki=0.0, roll_Kd=0.2;
//double pitch_Kp=2, pitch_Ki=0.1, pitch_Kd=0.0;
//double yaw_Kp=5, yaw_Ki=0.05, yaw_Kd=0.0;


//Specify the links and initial tuning parameters
PID roll_pid(&roll, &roll_u, &roll_s, roll_Kp, roll_Ki, roll_Kd, DIRECT);
//PID pitch_pid(&pitch, &pitch_u, &pitch_s, pitch_Kp, pitch_Ki, pitch_Kd, DIRECT);
//PID yaw_pid(&yaw, &yaw_u, &yaw_s, yaw_Kp, yaw_Ki, yaw_Kd, DIRECT);
const int sample_time = 10;  // in ms


void initSensors(){
  // Acell Sensor
  if(!accel.begin()) {
    // There was a problem detecting the LSM303 ... check your connections
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }

  // Gyro Sensor
  if(!gyro.begin())
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while(1);
  }
  /*if(!mag.begin()) {
    // There was a problem detecting the LSM303 ... check your connections
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }*/}


void setup()
{
  Serial.begin(9600);
  analogWrite(10,0);
  analogWrite(11, 0);
  delay(2000);
  // Init PID controllers
  roll_pid.SetMode(AUTOMATIC);
  roll_pid.SetSampleTime(sample_time);
  roll_pid.SetOutputLimits(roll_min, roll_max);
  // init sensor reading
  initSensors();
  // init filter

  // Delay for the motor to enter working mode
  delay(2500);

}

void readSensor()
{
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_vec_t   orientation;
  sensors_event_t gyro_event; 

  // Calculate pitch and roll from the raw accelerometer data.
  accel.getEvent(&accel_event);
  acceleration_x = accel_event.acceleration.x;
  mag.getEvent(&mag_event);
  gyro.getEvent(&gyro_event);

  if (dof.accelGetOrientation(&accel_event, &orientation))
  { 
    theta_acc = -orientation.roll-3.5;
  
  }
  
  // Read gyro output
  gyro_x = -1*180*gyro_event.gyro.x/M_PI;
  //roll_filter.SetInputs(roll,gyro_x);
  //roll_filter.Compute();
  //roll = roll_filter.GetOutput();

  
}

// gyro_angle() function is written temporally apart to obtain the angle from gyro => to be added to read_sensor().
double gyro_angle(){
  gyro_x /=(1000/interval);
  current_angle = current_angle + gyro_x; // the value substracted from the current_angle variable is due to the drift estimated of course
  return  current_angle;
}

double complementary_filter(){

    double    theta_err = roll - theta_acc;
              theta_err /=(1000/interval);
    double    integ_acc = integ_acc + theta_err;

    double    diff = gyro_x - (200*theta_err + 40*integ_acc);
              diff /=(1000/interval);
              roll = roll + diff;
return roll;

}


void loop()
{
  unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      readSensor();

      //Serial.print(roll);Serial.print(" ");Serial.println(gyro_angle());

      roll = complementary_filter();
      roll_pid.Compute();

       // Apply inputs
     // motor1_u = thrust + roll_u + 140; // - yaw_u;
      motor1_u = thrust + roll_u + 140; // - yaw_u;
     // motor2_u = thrust - pitch_u + 168; // + yaw_u;
      motor3_u = thrust - roll_u + 162; // - yaw_u;
     // motor4_u = thrust + pitch_u + 141; // + yaw_u;
     analogWrite(10, motor3_u);
     analogWrite(12, motor1_u);
     
    //Serial.print(theta_acc);Serial.print(" ");Serial.print(gyro_angle());Serial.print(" ");Serial.println(roll);
    //Serial.println(gyro_angle());
    Serial.print(roll);Serial.print(" ");Serial.print(motor1_u);Serial.print(" ");Serial.println(motor3_u);
  }


}

int Ajustar(int variable, int minimo, int maximo){ //Ajusta una variable entre dos valores
  if (variable > maximo) variable=maximo;
  if (variable < minimo) variable=minimo;
  return variable;
} 
