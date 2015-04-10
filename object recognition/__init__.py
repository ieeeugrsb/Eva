# -*- coding: utf-8 -*-

"""Main script."""

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
import color_detector
import shape_extraction
import logging

logger = logging.getLogger(__name__)
VIDEO_DEVICE = 1

def main():

    def circle():
        contour.fit_circle()
        cv2.imshow('circle',contour.draw_circle(image=frame.copy()))

    def rectangle():
        contour.fit_rectangle()
        cv2.imshow('rectangle',contour.draw_rectangle(image=frame.copy()))

    source = cv2.VideoCapture(VIDEO_DEVICE)
    
    looping = False
    if source.isOpened():
        looping = True
    
    while looping:
        # Handle exit
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
        # Get frame
        ret_val = False
        frame = None
        if source.isOpened():
            source.grab()
            ret_val, frame = source.retrieve()
            if ret_val is not True:
                Logger.error("Frame cannot be read")
                break
        
        # Detect color
        res, mask = color_detector.detect_red_blob(frame)
        
        if frame is not None:
            cv2.imshow('frame',frame)
        if mask is not None:
            cv2.imshow('mask',mask)
        
        # Extract contour
        contour = shape_extraction.Contour(mask.copy())
        
        contour.find()
        cv2.imshow('contours',contour.draw())
        rectangle()

if __name__ == "__main__":
    logging.getLogger(__name__).setLevel(logging.INFO)
    main()





