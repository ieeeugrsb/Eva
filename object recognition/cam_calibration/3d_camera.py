# -*- coding: utf-8 -*-

"""3D camera"""

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
        self.left_source = cv2.VideoCapture(left_device)
        if not self.left_source.isOpened():
            raise RuntimeError("Left video source cannot be opened")
        self.right_source = cv2.VideoCapture(right_device)
        if not self.right_source.isOpened():
            raise RuntimeError("Right video source cannot be opened")

        self.left_source.set(cv2.cv.CV_CAP_PROP_FRAME_WIDTH, size[0])
        self.left_source.set(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT, size[1])

        self.right_source.set(cv2.cv.CV_CAP_PROP_FRAME_WIDTH, size[0])
        self.right_source.set(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT, size[1])

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

def main():
    """Take 3D photos"""
    #leer camara
    cap = DualCamera(left_device=1, right_device=2)
    
    numero_foto=0

    while True:
        # Capture frame-by-frame
        left_f, right_f = cap.get_frames()

        # Display the resulting frame
        cv2.imshow('frame_left', left_f)
        cv2.imshow('frame_right', right_f)
        
        tecla = cv2.waitKey(1)
         
        if tecla & 0xFF == ord('q'):
            break
        if tecla & 0xFF == ord(' '):
            photo_left_name="calib_img/left"+str("{:02d}").format(numero_foto)+".jpg"
            photo_right_name="calib_img/right"+str("{:02d}").format(numero_foto)+".jpg"
            cv2.imwrite(photo_left_name, left_f)
            cv2.imwrite(photo_right_name, right_f)
            numero_foto += 1;
            with open("list.txt", "a") as f:
                f.write(photo_left_name + "\n")
                f.write(photo_right_name + "\n")
            

    # When everything done, release the capture
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
