/*
  Drone Control Software

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

#include <math.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>

#include <PID_v1.h>

#include <FlexiTimer2.h>

#include "Motor.h"

// Assign a unique ID to the sensors.
Adafruit_9DOF                 dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);

// Create motors instance and start in Working-Mode.
// See wiki to check motor positions
Motor motor_1(12);
Motor motor_2(11);
Motor motor_3(10);
Motor motor_4(9);

double thrust = 140;
double roll_s = 0, roll, roll_u;
double pitch_s = 0, pitch, pitch_u;
double yaw_s = 0, yaw, yaw_u;
int motor1_u, motor2_u, motor3_u, motor4_u;

//Define tuning parameters
double roll_Kp=5, roll_Ki=0.05, roll_Kd=0.0;
double pitch_Kp=5, pitch_Ki=0.05, pitch_Kd=0.0;
double yaw_Kp=5, yaw_Ki=0.05, yaw_Kd=0.0;

// Output limits
const double roll_min=-75, roll_max=75;
const double pitch_min=-75, pitch_max=75;
const double yaw_min=-75, yaw_max=75;

//Specify the links and initial tuning parameters
PID roll_pid(&roll, &roll_u, &roll_s, roll_Kp, roll_Ki, roll_Kd, DIRECT);
PID pitch_pid(&pitch, &pitch_u, &pitch_s, pitch_Kp, pitch_Ki, pitch_Kd, DIRECT);
PID yaw_pid(&yaw, &yaw_u, &yaw_s, yaw_Kp, yaw_Ki, yaw_Kd, DIRECT);
const unsigned int main_sample_time = 10;  // in ms

//Print freq counter
const int print_sample_time = 1000;  // in ms
unsigned long print_freq_count= 0;

//loop control
bool next_step = true;


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

    // Set internal timer
    FlexiTimer2::set(main_sample_time, activateStep);

    // Init sensors
    initSensors();

    // Init PID controllers
    roll_pid.SetMode(AUTOMATIC);
    roll_pid.SetSampleTime(main_sample_time);
    roll_pid.SetOutputLimits(roll_min, roll_max);
    pitch_pid.SetMode(AUTOMATIC);
    pitch_pid.SetSampleTime(main_sample_time);
    pitch_pid.SetOutputLimits(pitch_min, pitch_max);
    yaw_pid.SetMode(AUTOMATIC);
    yaw_pid.SetSampleTime(main_sample_time);
    yaw_pid.SetOutputLimits(yaw_min, yaw_max);

    // Wait for motors going to working mode.
    delay(2000);

    // Start timer
    FlexiTimer2::start();

}

void activateStep()
{
    next_step = true;
}

void readSensor()
{
    sensors_event_t accel_event;
    sensors_event_t mag_event;
    sensors_vec_t   orientation;

    // Calculate pitch and roll from the raw accelerometer data.
    accel.getEvent(&accel_event);
    mag.getEvent(&mag_event);
    if (dof.accelGetOrientation(&accel_event, &orientation) &&
        dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
    {
        roll = orientation.roll;
        pitch = orientation.pitch;
        if (pitch >= 0) pitch = 180 - pitch;
        if (pitch < 0) pitch = -180 - pitch;
        yaw = -orientation.heading;
    }
}

void loop()
{
    if (next_step == true)
    {
        next_step = false;
        attitudeControlLoop();

        if (print_freq_count >= print_sample_time / main_sample_time)
        {
            showSensorData();
            showControlInputs();
            print_freq_count = 0;
        }
        else
        {
            ++print_freq_count;
        }
    }
}

void attitudeControlLoop()
{
    // Measure states
    readSensor();

    // Print sensor data
    showSensorData();

    // Compute control inputs
    // PID library decides itself when its time to update
    // its output according to sample time
    roll_pid.Compute();
    pitch_pid.Compute();
    //yaw_pid.Compute();

    // Apply inputs
    motor1_u = thrust + roll_u; // - yaw_u;
    motor2_u = thrust - pitch_u; // + yaw_u;
    motor3_u = thrust - roll_u; // - yaw_u;
    motor4_u = thrust + pitch_u; // + yaw_u;

    showControlInputs();

    motor_1.write(motor1_u);
    motor_2.write(motor2_u);
    motor_3.write(motor3_u);
    motor_4.write(motor4_u);
}

void showSensorData()
{
    Serial.print("Roll: ");
    Serial.print(roll);
    Serial.print("; ");
    Serial.print("Pitch: ");
    Serial.print(pitch);
    Serial.print("; ");
    Serial.print("Heading: ");
    Serial.print(yaw);
    Serial.print("; ");
    Serial.println("");
}

void showControlInputs()
{
    Serial.print("roll_u: ");
    Serial.print(roll_u);
    Serial.print("; ");
    Serial.print("pitch_u: ");
    Serial.print(pitch_u);
    Serial.print("; ");
    Serial.print("yaw_u: ");
    Serial.print(yaw_u);
    Serial.print("; ");
    Serial.println("");

    Serial.print("m1: ");
    Serial.print(motor1_u);
    Serial.print("; ");
    Serial.print("m2: ");
    Serial.print(motor2_u);
    Serial.print("; ");
    Serial.print("m3: ");
    Serial.print(motor3_u);
    Serial.print("; ");
    Serial.print("m4: ");
    Serial.print(motor4_u);
    Serial.print("; ");
    Serial.println("");
}
