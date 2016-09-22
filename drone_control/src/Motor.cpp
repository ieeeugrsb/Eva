/*
  Drone Control Software - Motor Library for ESC control.

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

#include "Arduino.h"
#include "Motor.h"

Motor::Motor(int pin, int zero_val, int max_val)
{
    // There is no need to set pinMode for PWM signals.
    this->pin = pin;
    this->MOTOR_ZERO = constrain(zero_val, 0, 255);
    this->MOTOR_MAX = constrain(max_val, 0, 255);

    // Set to working mode going to zero position.
    analogWrite(pin, 0);
}

void Motor::stop()
{
    analogWrite(this->pin, 0);
}

int Motor::write(int value)
{
    int final_value = constrain(value + MOTOR_ZERO, MOTOR_ZERO, MOTOR_MAX);

    analogWrite(this->pin, final_value);

    return final_value;
}
