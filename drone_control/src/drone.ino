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

#include "Motor.h"

// Create motors instance and start in Working-Mode.
Motor motor_bl(12); // Back-Left Motor
Motor motor_br(11); // Back-Right Motor
Motor motor_fl(10); // Front-Left Motor
Motor motor_fr( 9); // Front-Right Motor

int value = 0;
int incremento = 5;

void write2motors(int value)
{
    Serial.print("Writing "); Serial.println(value);
    motor_bl.write(value + 14);
    motor_br.write(value + 15);
    motor_fl.write(value);
    motor_fr.write(value);
}

void setup()
{
    // Setup PC communication.
    Serial.begin(9600);

    // Wait for motors going to working mode.
    delay(2000);
}

void loop()
{
    if (value > 25 || value < 0)
        incremento *= -1;

    value += incremento;
    write2motors(value);

    delay(2000);
}
