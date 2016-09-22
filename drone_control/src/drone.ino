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

#include "Common.h"
#include "Motor.h"
#include "IMUComplementaryFilter.h"

// Assign a unique ID to the sensors.
Adafruit_9DOF                 dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(30303);

// Orientation angle filters
IMUComplementaryFilter roll_filter = IMUComplementaryFilter(ROLL_FILTER_KP,
    ROLL_FILTER_KI, ATTITUDE_LOOP_PERIOD);
IMUComplementaryFilter pitch_filter = IMUComplementaryFilter(PITCH_FILTER_KP,
    PITCH_FILTER_KI, ATTITUDE_LOOP_PERIOD);

// Create motors instance and start in Working-Mode.
// See wiki to check motor positions
Motor motor_1(MOTOR_1_PIN, MOTOR_1_ZERO, MOTOR_1_MAX);
Motor motor_2(MOTOR_2_PIN, MOTOR_2_ZERO, MOTOR_2_MAX);
Motor motor_3(MOTOR_3_PIN, MOTOR_3_ZERO, MOTOR_3_MAX);
Motor motor_4(MOTOR_4_PIN, MOTOR_4_ZERO, MOTOR_4_MAX);
// Do not use pins 9 and 10 because Timer2 is already being used.

double thrust = 0;
double roll_s = 0, roll=0, roll_u=0;
double pitch_s = 0, pitch=0, pitch_u=0;
double yaw_s = 0, yaw=0, yaw_u=0;
int motor1_u=0, motor2_u=0, motor3_u=0, motor4_u=0;

double roll_acc = 0;
double pitch_acc = 0;

double gyro_x = 0;
double gyro_y = 0;

//Define tuning parameters
double roll_Kp=ROLL_PID_KP, roll_Ki=ROLL_PID_KI, roll_Kd=ROLL_PID_KD;
double pitch_Kp=PITCH_PID_KP, pitch_Ki=PITCH_PID_KI, pitch_Kd=PITCH_PID_KD;
double yaw_Kp=YAW_PID_KP, yaw_Ki=YAW_PID_KI, yaw_Kd=YAW_PID_KD;

// Output limits
const double roll_min=-75, roll_max=75;
const double pitch_min=-75, pitch_max=75;
const double yaw_min=-75, yaw_max=75;

//Specify the links and initial tuning parameters
PID roll_pid(&roll, &roll_u, &roll_s, roll_Kp, roll_Ki, roll_Kd, DIRECT);
PID pitch_pid(&pitch, &pitch_u, &pitch_s, pitch_Kp, pitch_Ki, pitch_Kd, DIRECT);
PID yaw_pid(&yaw, &yaw_u, &yaw_s, yaw_Kp, yaw_Ki, yaw_Kd, DIRECT);

//loop control
unsigned long last_attitude_loop_time = 0;


void initSensors()
{
    if(!accel.begin()) {
        #ifdef DEBUG
        // There was a problem detecting the LSM303 ... check your connections
        Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
        #endif
        while(1);
    }

    if(!mag.begin()) {
        #ifdef DEBUG
        // There was a problem detecting the LSM303 ... check your connections
        Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
        #endif
        while(1);
    }

    if(!gyro.begin()) {
        #ifdef DEBUG
        // There was a problem detecting the LSM303 ... check your connections
        Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
        #endif
        while(1);
    }
}

void setup()
{
    // Setup PC communication.
    Serial.begin(9600);

    // Init sensors
    initSensors();

    // Init PID controllers
    roll_pid.SetMode(AUTOMATIC);
    roll_pid.SetSampleTime(ATTITUDE_LOOP_PERIOD);
    roll_pid.SetOutputLimits(roll_min, roll_max);
    pitch_pid.SetMode(AUTOMATIC);
    pitch_pid.SetSampleTime(ATTITUDE_LOOP_PERIOD);
    pitch_pid.SetOutputLimits(pitch_min, pitch_max);
    yaw_pid.SetMode(AUTOMATIC);
    yaw_pid.SetSampleTime(ATTITUDE_LOOP_PERIOD);
    yaw_pid.SetOutputLimits(yaw_min, yaw_max);

    // Wait for motors going to working mode.
    delay(2000);
}

void readSensor()
{
    sensors_event_t accel_event;
    sensors_event_t mag_event;
    sensors_event_t gyro_event;
    sensors_vec_t   orientation;

    // Calculate pitch and roll from the raw accelerometer data.
    accel.getEvent(&accel_event);
    mag.getEvent(&mag_event);
    gyro.getEvent(&gyro_event);

    if (dof.accelGetOrientation(&accel_event, &orientation) &&
        dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
    {
        roll_acc = orientation.roll;
        pitch_acc = orientation.pitch;
        if (pitch_acc >= 0) pitch_acc = M_PI - pitch_acc;
        if (pitch_acc < 0) pitch_acc = -M_PI - pitch_acc;
        yaw = -orientation.heading;

        gyro_x = -1*180*gyro_event.gyro.x/M_PI;
        gyro_y = 1*180*gyro_event.gyro.y/M_PI;

        roll_filter.SetInputs(roll_acc, gyro_x);
        pitch_filter.SetInputs(pitch_acc, gyro_y);
        roll_filter.Compute();
        pitch_filter.Compute();
        roll = roll_filter.GetOutput();
        pitch = pitch_filter.GetOutput();
    }
}

void loop()
{
    // Thrust selection
    if (Serial.available() > 0) {
        int command = Serial.read();
        thrust = Serial.parseInt();
    }


    // Do iteration or pass if next iteration doesn't have to start yet
    unsigned long time_now = millis();
    if(time_now - last_attitude_loop_time >= ATTITUDE_LOOP_PERIOD)
    {
        last_attitude_loop_time = time_now;

        // read orientation
        readSensor();

        // Compute control inputs
        // PID library decides itself when its time to update
        // its output according to sample time
        roll_pid.Compute();
        // TODO: Uncomment to enable
        // pitch_pid.Compute();
        // yaw_pid.Compute();

        // Apply inputs
        motor1_u = thrust + roll_u - yaw_u;
        motor2_u = thrust - pitch_u + yaw_u;
        motor3_u = thrust - roll_u - yaw_u;
        motor4_u = thrust + pitch_u + yaw_u;

        motor2_u = 0;
        motor4_u = 0;

        int u_1 = motor_1.write(motor1_u);
        int u_2 = motor_2.write(motor2_u);
        int u_3 = motor_3.write(motor3_u);
        int u_4 = motor_4.write(motor4_u);

        #ifdef DEBUG
        Serial.print(roll);Serial.print(" ");Serial.print(motor1_u);Serial.print(" ");Serial.print(motor3_u);Serial.print(" ");Serial.print(u_1);Serial.print(" ");Serial.println(u_3);
        #endif
    }
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
