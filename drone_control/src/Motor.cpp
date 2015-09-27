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

Motor::Motor(int pin)
{
    // There is no need to set pinMode for PWM signals.
    this->pin = pin;

    // Set to working mode going to zero position.
    analogWrite(pin, MOTOR_ZERO);
}

void Motor::stop()
{
    analogWrite(this->pin, MOTOR_ZERO);
}

void Motor::write(int value)
{
    int final_value = MOTOR_ZERO + value;
    if (final_value < MOTOR_ZERO || final_value > MOTOR_MAX)
        return;

    analogWrite(this->pin, final_value);
}
