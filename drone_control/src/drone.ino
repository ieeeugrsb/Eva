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

const int MOTOR_BL = 12;    // PWM pin for Back-Left Motor
const int MOTOR_BR = 11;    // PWM pin for Back-Right Motor
const int MOTOR_FL = 10;    // PWM pin for Front-Left Motor
const int MOTOR_FR =  9;    // PWM pin for Front-Right Motor

int value = 140;
int incremento = 5;

void write2motors(int value)
{
    Serial.print("Writing "); Serial.println(value);
    analogWrite(MOTOR_BL, value);
    analogWrite(MOTOR_BR, value);
    analogWrite(MOTOR_FL, value);
    analogWrite(MOTOR_FR, value);
}

void setup_motors()
{
    // It's not necessary to set pin mode to use PWM.

    // Set motors to position 0.
    write2motors(value);
}

void setup()
{
    // Setup PC communication.
    Serial.begin(9600);

    // Setup motors
    setup_motors();
}

void test_motor()
{
    int data = Serial.parseInt();
    if (data <= 0)
        return;

    Serial.print("\nSending ");
    Serial.print(data);
    Serial.println(" to motor.");

    analogWrite(MOTOR_FL, data);
}

void loop()
{
    delay(2000);
    value += incremento;
    write2motors(value);

    if (value > 170 || value < 140) {
        incremento *= -1;
    }
}
