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

import cv2


class DualCamera(object):
    
    def __init__(self, left_device=0, right_device=1):
        self.left_source = cv2.VideoCapture(left_device)
        if not self.left_source.isOpened():
            raise RuntimeError("Left video source cannot be openned")
        self.right_source = cv2.VideoCapture(right_device)
        if not self.right_source.isOpened():
            raise RuntimeError("Right video source cannot be openned")      
    
    def get_frames(self):
        """Get left and right frames.

        Returns:
           array.  left frame
           array.  right frame
    
        Raises:
           RuntimeError
       """

        left_frame = None
        right_frame = None
        if self.left_source.isOpened():
            left_ret_val, left_frame = self.left_source.read()
            if left_ret_val is not True:
                raise RuntimeError("Left frame cannot be readed")
        else:
            raise RuntimeError("Left video source is closed")
        
        if self.right_source.isOpened():
            right_ret_val, right_frame = self.right_source.read()
            if right_ret_val is not True:
                raise RuntimeError("Right frame cannot be readed")
        else:
            raise RuntimeError("Right video source is closed")
        
        return left_frame, right_frame
    
    def release(self):
        """Close video sources."""
        self.left_source.release()
        self.right_source.release()

if __name__ == "__main__":
    #leer camara
    cap = DualCamera()
    
    while(True):
        # Capture frame-by-frame
        left_frame, right_frame = cap.get_frames()
    
        # Our operations on the frame come here
        gray_left = cv2.cvtColor(left_frame, cv2.COLOR_BGR2GRAY)
    
        # Display the resulting frame
        cv2.imshow('frame_left',gray_left)
        cv2.imshow('frame_right',right_frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    # When everything done, release the capture
    cap.release()
    cv2.destroyAllWindows()