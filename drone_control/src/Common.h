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

#endif
