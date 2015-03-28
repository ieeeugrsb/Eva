#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Eva - Object recogniton
# Copyright (C) 2014 Rafael Bailón-Ruiz <rafaelbailon@ieee.org>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# By downloading, copying, installing or using the software you agree to this license.
# If you do not agree to this license, do not download, install,
# copy or use the software.

from __future__ import print_function

import abc


class Process(object):
    """Base class for processes.

    Attributes:
        step_time: minimum time between processing steps in ms.
        working: True if precssing is working. False if stopped.
    """
    __metaclass__ = abc.ABCMeta

    def __init__(self, step_time=1000):
        """Inits ObjectFinder.

        Args:
            step_time: minimum time between processing steps in ms.
            is_working: True if precssing is working. False if stopped.
        """
        self.step_time = step_time
        self.is_working = False

    def start(self):
        """Sets process as working."""
        self.is_working=True

    def stop(self):
        """Sets process as stopped."""
        self.is_working=False

    @abc.abstractmethod
    def do(self):
        """Do process.

        Abstract method.
        """
        return


class ObjectFinder(Process):
    """Find object using image features.

    Attributes:
        found_cb: function called when the object is found. (#TODO: determine its arguments)
    """

    def __init__(self, target, data_input, found_cb, step_time=1000):
        """Inits ObjectFinder.

        Args:
            target: image of object to be found.
            data_input: function returning input image where object is going to be found.
            step_time: minimum time between processing steps in ms.
            found_cb: function called when the object is found.
        """
        super(ObjectFinder,self).__init__(step_time)
        self.found_cb = found_cb

    def do(self):
        """Do object finding."""
        super(ObjectFinder,self).do()
        raise NotImplementedError()

class TestProcess(Process):
    """TestProcess.

    Args:
        name: Name.
        count: Count.
        count_limit: Count limit.
    """

    def __init__(self, name, count_limit, step_time=1000):
        """Inits TestProcess.
        Args:
            name: Name.
            count_limit: Count limit.
        """
        super(TestProcess,self).__init__(step_time)
        self.name = name
        self.count = 0
        self.count_limit = count_limit

    def start(self):
        """Starts counting.

        Sets count to 0.
        """
        super(TestProcess,self).start(self)
        self.count = 0

    def do(self):
        """Does count."""

        self.count += 1
        print("Count[" + self.name + "]: " + str(self.count))

class ProcessManager(object):
    """Manage processes.

    Attributes:
        processes: list of managed processes. They are executed in order.
        is_running: True if ProcessManager is running.
    """
    def __init__(self, processes):
        """Inits ProcessManager.

        Args:
            processes: List of processes.
        """
        self.processes = processes
        self.is_running = False

    def run(self, auto_start=True):
        """Run processes at their defined intervals.

        Args:
            auto_start: Start all assigned processes (optional).
        """
        if auto_start:
            for process in self.processes:
                process.start()

        self.is_running = True

        while self.is_running:
            #TODO: Handle intervals.
            for process in self.processes:
                if process.is_working:
                    process.do()

if __name__ == "__main__":
    processes = [TestProcess("1", 100), TestProcess("2", 200), TestProcess("3", 300)]
    manager = ProcessManager(processes)
    manager.run()
