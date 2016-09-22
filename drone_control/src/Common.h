/*
  Drone Control Software - Common definitions.

  Copyright (C) 2016 Rafael Bailón-Ruiz <rafaelbailon@ieee.org>

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

#ifndef Common_h
#define Common_h

#define DEBUG

#define MAIN_LOOP_PERIOD 10  // in ms
#define ATTITUDE_LOOP_MULT 1
#define ATTITUDE_LOOP_PERIOD MAIN_LOOP_PERIOD*ATTITUDE_LOOP_MULT
#define PRINT_LOOP_MULT 100
#define PRINT_LOOP_PERIOD MAIN_LOOP_PERIOD*PRINT_LOOP_MULT

// Motor control pins
#define MOTOR_1_PIN 10
#define MOTOR_2_PIN 11
#define MOTOR_3_PIN 12
#define MOTOR_4_PIN 9

// Orientation sensor offset
#define ROLL_OFFSET -3.5
#define PITCH_OFFSET 0.0

// Roll sensor complementary filter
#define ROLL_FILTER_KP 50
#define ROLL_FILTER_KI 0.01

// Pitch sensor complementary filter
#define PITCH_FILTER_KP 200.0
#define PITCH_FILTER_KI 40.0

// Roll PID parameters
#define ROLL_PID_KP 0.4
#define ROLL_PID_KI 0.0
#define ROLL_PID_KD 0.0

// Pitch PID parameters
#define PITCH_PID_KP 0.0
#define PITCH_PID_KI 0.0
#define PITCH_PID_KD 0.0

// Yaw PID parameteres
#define YAW_PID_KP 0.0
#define YAW_PID_KI 0.0
#define YAW_PID_KD 0.0

#endif
