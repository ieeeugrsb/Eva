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

# References:
# http://stackoverflow.com/questions/20912948/color-detection-using-opencv-python
# http://www.pyimagesearch.com/2014/08/04/opencv-python-color-detection/

import cv2
import numpy as np


def color_detector(src, color_lower, color_upper):
    '''color_detector(src, color_lower, color_upper) -> result_img, mask

    src: image on RGB space

    color_lower: lower color threshold in HSV space

    color_upper: upper color threshold in HSV space
    '''
    # Convert image to HSV space
    hsv = cv2.cvtColor(src, cv2.COLOR_BGR2HSV)

    # Threshold the HSV image to get only selected colors
    mask = cv2.inRange(hsv, color_lower, color_upper)

    # Erosion + dilation to reduce noise
    kernel = np.ones((1, 1), np.uint8)
    mask_dilated = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)

    # Bitwise-AND mask and original image
    result_img = cv2.bitwise_and(src, src, mask=mask_dilated)

    return result_img, mask_dilated

def detect_blue_blob(frame):
    '''
    Finds blue blobs on image and returns a masked image and its mask
    '''
    # define range of blue color in HSV
    #lower_blue = np.array([110,50,50])
    #upper_blue = np.array([130,255,255])

    lower_blue = np.array([85, 75, 75])
    upper_blue = np.array([130, 255, 255])

    return color_detector(frame, lower_blue, upper_blue)

def detect_red_blob(frame):
    '''
    Finds red blobs on image and returns a masked image and its mask
    '''
    # define range of red color in HSV
    # WARNING: red color hue wraps around 0 on HSV

    lower_red_start = np.array([0,75,75])
    upper_red_start = np.array([10,255,255])

    lower_red_end = np.array([165,75,75])
    upper_red_end = np.array([180,255,255])

    result_img_start, mask_start =  color_detector(frame, lower_red_start, upper_red_start)
    result_img_end, mask_end =  color_detector(frame, lower_red_end, upper_red_end)

    # Add masks

    mask = cv2.bitwise_or(mask_start, mask_end)
    result_img = cv2.bitwise_and(frame, frame, mask=mask)

    return result_img, mask


def detect_green_blob(frame):
    '''
    Finds green blobs on image and returns a masked image and its mask
    '''
    # define range of green color in HSV

    lower_green = np.array([90,75,75])
    upper_green = np.array([130,255,255])

    return color_detector(frame, lower_green, upper_green)

def detect_yellow_blob(frame):
    '''
    Finds green blobs on image and returns a masked image and its mask
    '''
    # define range of yellow color in HSV

    lower_yellow = np.array([30,50,75])
    upper_yellow = np.array([80,255,255])

    return color_detector(frame, lower_yellow, upper_yellow)

if __name__ == "__main__":

    cap = cv2.VideoCapture(0)

    while(True):
        ret, frame = cap.read()

        res, mask, = detect_red_blob(frame)

        cv2.imshow('frame',frame)
        cv2.imshow('mask',mask)
        cv2.imshow('res',res)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # When everything done, release the capture
    cap.release()
    cv2.destroyAllWindows()
