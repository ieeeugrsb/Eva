/*
  Drone Control Software - Attitude estimation using a complementary filter.

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

#ifndef IMUCOMPLEMENTARYFILTER_H
#define IMUCOMPLEMENTARYFILTER_H


class IMUComplementaryFilter
{
    public:
        IMUComplementaryFilter(double kp, double ki, int sample_time);
        void SetInputs(double theta_acc, double theta_dot);
        void Compute();
        double GetOutput();

    private:
        double kp;
        double ki;
        int sample_time;  // in ms
        double gyro_integrator;
        double drift_integrator;
        double theta_acc;  // in rad
        double theta_dot;  // in rad
        double theta_f;  // in rad
};

#endif
