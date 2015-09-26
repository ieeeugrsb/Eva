/*
  Drone Control Software - Test each motor at the lower speed.

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

const int MOTOR_BL = 12;    // PWM pin for Back-Left Motor
const int MOTOR_BR = 11;    // PWM pin for Back-Right Motor
const int MOTOR_FL = 10;    // PWM pin for Front-Left Motor
const int MOTOR_FR =  9;    // PWM pin for Front-Right Motor
const int PIN_MOTORS[4] = { MOTOR_BL, MOTOR_FR, MOTOR_BR, MOTOR_FL };

const int PWR_MIN = 140;

void write2motor(int pin, int value)
{
    Serial.print("Motor "); Serial.print(pin);
    Serial.print(" -> ");   Serial.println(value);
    analogWrite(pin, value);
}

void setup_motors()
{
    // It's not necessary to set pin mode to use PWM.
    // Enter into working mode going to 0.
    for (int i = 0; i < 4; i++)
        analogWrite(PIN_MOTORS[i], PWR_MIN);
    delay(2500);
}

void setup()
{
    // Setup PC communication.
    Serial.begin(9600);

    // Setup motors
    setup_motors();
}

void loop()
{
    for (int i = 0; i < 4; i++) {
        // Start motor and wait.
        write2motor(PIN_MOTORS[i], PWR_MIN + 2);
        delay(2500);

        // Stop motor and wait.
        write2motor(PIN_MOTORS[i], PWR_MIN);
        delay(2500);
    }
}
