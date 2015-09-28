/*
  Drone Control Software - Specify the PWM signal of one motor by serial.

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

const int MOTOR = MOTOR_FL;

void setup()
{
    // Setup PC communication.
    Serial.begin(9600);

    // Setup motors: Must be done manually sending the value 140 and waiting.
}

String data = "";

void loop()
{
    int value;
    while (Serial.available() > 0) {
        value = Serial.read();
        if (isDigit(value))
            data += (char)value;
    }

    if (value == '\n') {
        int pwm_value = data.toInt();

        Serial.print("Sending ");
        Serial.print(pwm_value);
        Serial.println(" to motor.");

        analogWrite(MOTOR, pwm_value);

        data = "";
    }
}
