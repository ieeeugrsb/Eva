# -*- coding: utf-8 -*-

"""3D photo module for camera calibration."""

# Eva - Object recogniton
# Copyright (C) 2014-2015 Rafael Bailón-Ruiz <rafaelbailon@ieee.org>
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

import argparse
import logging
import cv2

logger = logging.getLogger(__name__)

VIDEO_DEVICE_L = 0
VIDEO_DEVICE_R = 1

class DualCamera(object):
    """Wrapper object for cameras with two video sources.

    Attributes:
        left_source: cv2.VideoCapture object.
        right_source: cv2.VideoCapture object.
    """

    def __init__(self, left_device=0, right_device=1, size=(320, 240)):
        """Inits DualCamera.

        Args:
            left_device: Left video source number.
            right_device: Right video source number.

        Raises:
            RuntimeError: A video source cannot be opened.
        """
        self.left_source = cv2.VideoCapture()
        self.left_source.set(cv2.cv.CV_CAP_PROP_FRAME_WIDTH, size[0])
        self.left_source.set(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT, size[1])
        self.left_source.open(left_device)
        if not self.left_source.isOpened():
            raise RuntimeError("Left video source cannot be opened")

        self.right_source = cv2.VideoCapture()
        self.right_source.set(cv2.cv.CV_CAP_PROP_FRAME_WIDTH, size[0])
        self.right_source.set(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT, size[1])
        self.right_source.open(right_device)
        if not self.right_source.isOpened():
            raise RuntimeError("Right video source cannot be opened")

    def get_frames(self):
        """Gets left and right frames.

        Returns:
           array: Left frame
           array: Right frame

        Raises:
           RuntimeError: Frame cannot be read
       """

        left_grab_ret_val = self.left_source.grab()
        right_grab_ret_val = self.right_source.grab()

        left_frame = None
        right_frame = None
        if self.left_source.isOpened():
            left_ret_val, left_frame = self.left_source.retrieve()
            if left_ret_val is not True:
                pass
#                raise RuntimeError("Left frame cannot be read")
        else:
            raise RuntimeError("Left video source is closed")

        if self.right_source.isOpened():
            right_ret_val, right_frame = self.right_source.retrieve()
            if right_ret_val is not True:
                pass
#                raise RuntimeError("Right frame cannot be read")
        else:
            raise RuntimeError("Right video source is closed")

        return left_frame, right_frame

    def release(self):
        """Closes video sources."""
        self.left_source.release()
        self.right_source.release()

def _test():
    """DualCamera test function"""
    #leer camara
    cap = DualCamera(left_device=0, right_device=1)

    while True:
        # Capture frame-by-frame
        left_f, right_f = cap.get_frames()

        # Our operations on the frame come here
        gray_left = cv2.cvtColor(left_f, cv2.COLOR_BGR2GRAY)

        # Display the resulting frame
        cv2.imshow('frame_left', gray_left)
        cv2.imshow('frame_right', right_f)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # When everything done, release the capture
    cap.release()
    cv2.destroyAllWindows()

def main():
    
    looping = False
    cap = None
    try:
        cap = DualCamera(left_device=VIDEO_DEVICE_L, right_device=VIDEO_DEVICE_R)
        looping = True
    except RuntimeError as e:
        logger.error(e)

    while looping:
        # Get frame
        left_f, right_f = cap.get_frames()        
        cv2.imshow('left frame', left_f)
        cv2.imshow('right frame', right_f)
        
        # Handle exit
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    cap.release()
    cv2.destroyAllWindows()
        
if __name__ == "__main__":
    logger.setLevel(logging.INFO)
    main()





