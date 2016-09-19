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

#include "Common.h"
#include "Motor.h"
#include "IMUComplementaryFilter.h"

// Assign a unique ID to the sensors.
Adafruit_9DOF                 dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);

// Orientation angle filters
IMUComplementaryFilter roll_filter = IMUComplementaryFilter(0.01, 0.001,
    SENSOR_SAMPLE_TIME);
IMUComplementaryFilter pitch_filter = IMUComplementaryFilter(0.01, 0.001,
    SENSOR_SAMPLE_TIME);

// Create motors instance and start in Working-Mode.
// See wiki to check motor positions
Motor motor_1(12);
Motor motor_2(11);
Motor motor_3(8);
Motor motor_4(7);
// Do not use pins 9 and 10 because Timer2 is already being used.

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

//loop control
bool main_loop_step = true;
bool attitude_loop_step = true;
byte attitude_loop_count = 0;
bool print_loop_step = true;
byte print_loop_count = 0;


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
}

void setup()
{
    // Setup PC communication.
    Serial.begin(9600);

    // Set internal timer
    FlexiTimer2::set(MAIN_LOOP_PERIOD, onTimer2Event);

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

void onTimer2Event()
{
    main_loop_step = true;

    if (attitude_loop_count >= ATTITUDE_LOOP_MULT)
    {
        #ifdef DEBUG
        if (attitude_loop_step == true)
        {
            Serial.println("Last attitude loop iteration did not finish on "
            "time or you forgot to set attitude_loop_step to false");
        }
        #endif
        attitude_loop_step = true;
        attitude_loop_count = 0;
    }

    if (print_loop_count >= PRINT_LOOP_MULT)
    {
        #ifdef DEBUG
        if (print_loop_step == true)
        {
            Serial.println("Last print loop iteration did not finish on time "
            "or you forgot to set print_loop_step to false");
        }
        #endif
        print_loop_step = true;
        print_loop_count = 0;
    }
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
        roll = orientation.roll * M_PI / 180;
        pitch = orientation.pitch * M_PI / 180;
        if (pitch >= 0) pitch = M_PI - pitch;
        if (pitch < 0) pitch = -M_PI - pitch;
        yaw = -orientation.heading * M_PI / 180;

        roll_filter.SetInputs(roll, gyro_event.gyro.x)
        pitch_filter.SetInputs(pitch, gyro_event.gyro.y)
        roll_filter.Compute();
        pitch_filter.Compute();
        roll = roll_filter.GetOutput();
        pitch = pitch_filter.GetOutput();
    }
}

void loop()
{
    if (main_loop_step == true)
    {
        main_loop_step = false;
    }

    if (attitude_loop_step == true)
    {
        attitudeLoop();
        attitude_loop_step = false;
    }

    if (print_loop_step == true)
    {
        printLoop();
        print_loop_step = false;
    }
}

void attitudeLoop()
{
    // Measure states
    readSensor();

    #ifdef DEBUG
    // Print sensor data
    showSensorData();
    #endif

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

    #ifdef DEBUG
    showControlInputs();
    #endif

    motor_1.write(motor1_u);
    motor_2.write(motor2_u);
    motor_3.write(motor3_u);
    motor_4.write(motor4_u);
}

void printLoop()
{
    showSensorData();
    showControlInputs();
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
