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
#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

const int MOTOR_ZERO = 140;
const int MOTOR_MAX  = 254; // 255 enters configuration mode

class Motor
{
    public:
        Motor(int pin);
        void stop();
        void write(int value);

    private:
        int pin;
};

#endif
