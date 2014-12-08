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
import numpy as np
import matplotlib.pyplot as plt

import color_detector


im = cv2.imread('/home/rafael/src/staging/aprender-opencv/blob_detection/red_blob_src.jpg')
imgray = cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)

# Get binary image from color detection
masked, binary_mask = color_detector.detect_red_blob(im)

cv2.imshow('masked', binary_mask)

ret,thresh = cv2.threshold(imgray,127,255,0)

# find contours on binary image
contours, hierarchy = cv2.findContours(binary_mask,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)


# Draw extern contour
cnt_index = 0
cnt = contours[cnt_index]
im_contours = im.copy() # original image is modified by drawContours
cv2.drawContours(im_contours, [cnt], 0, (0,255,0), 3)

cv2.imshow('contour #' + str(cnt_index),im_contours)

# Now we try to fit a circle

(circ_x, circ_y), circ_radius = cv2.minEnclosingCircle(cnt)
# minEnclosingCircle returns floats
#center = (int(x),int(y))
#radius = int(radius)
print "(center_x, center_y) radius"
print (circ_x, circ_y), circ_radius

cv2.circle(im,(int(circ_x),int(circ_y)),int(circ_radius),(0,0,255),2)

cv2.imshow('bounding circle #' + str(cnt_index),im)

while True:
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
