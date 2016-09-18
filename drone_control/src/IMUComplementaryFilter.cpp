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

#include "IMUComplementaryFilter.h"


IMUComplementaryFilter::IMUComplementaryFilter(double kp, double ki,
                                               int sample_time)
{
    this->kp = kp;
    this->ki = ki;
    this->theta_f = 0.0;
    this->theta_dot = 0.0;
    this->theta_acc = 0.0;

    this->gyro_integrator = 0.0;
    this->drift_integrator = 0.0;
}

/*
Perform step in filtering loop

Uses forward euler integration
*/
void IMUComplementaryFilter::Compute()
{
    int theta_err = theta_f - theta_acc;
    drift_integrator = drift_integrator + theta_err * sample_time;
    gyro_integrator = theta_dot - (theta_err * kp + drift_integrator * ki);
    theta_f = gyro_integrator;
}

/*
Set theta and theta_dot inputs in filter
*/
void IMUComplementaryFilter::SetInputs(double theta_acc, double theta_dot)
{
    this->theta_acc = theta_acc;
    this->theta_dot = theta_dot;
}

/*
Get theta angle estimation
*/
double IMUComplementaryFilter::GetOutput()
{
    return this->theta_f;
}
