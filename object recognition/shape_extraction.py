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

import logging

import cv2
import numpy as np
import matplotlib.pyplot as plt

import color_detector

class Contour(object):

    def __init__(self, image):
        '''Init Contour.
        
        Args:
            image: Binary image
        '''
        super(Contour,self).__init__()
        self.img = image
        self.cnt_index = 0
        self.contours = None
        self.hierarchy = None
        self.circ_pos = (0.0,0.0)
        self.circ_radius = 0.0

    def find(self):
        '''Find contours on binary image.
        
        Returns:
            contours: See cv2.findContours docs.
            hierarchy: See cv2.findContours docs.
        '''
        self.contours, self.hierarchy = cv2.findContours(self.img, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        return self.contours, self.hierarchy
    
    def draw(self, color=(0,255,0), image=None):
        '''Draw extern contour.
        
        Args:
            color: Optional. (b,g,r) tuple.
            image: Optional. If set, writes over image; else, writes over a copy of self.img. 
        
        Returns:
            img_contours: image with drawn contours
        '''
                # original image is modified by drawContours
        img_contours = image
        if img_contours is None:
            img_contours = self.img.copy()
        
        if self.cnt_index < len(self.contours):
            cnt = self.contours[self.cnt_index]
            cv2.drawContours(img_contours, [cnt], 0, color, 3)
        
        return img_contours
        
    def draw_circle(self, color=(0,0,255), image=None):
        '''Draw fitted circle.
        
        Args:
            color: Optional. (b,g,r) tuple.
            image: Optional. If set, writes over image; else, writes over a copy of self.img.         
        
        Returns:
            img_circle: image with drawn circle.
        '''
        # original image is modified by cv2.circle
        img_circle = image
        if img_circle is None:
            img_circle = self.img.copy()
        
        cv2.circle(img_circle, (int(self.circ_pos[0]), int(self.circ_pos[1])), int(self.circ_radius), color, 2)
        
        return img_circle
        
    def draw_rectangle(self, color=(0,0,255), image=None):
        '''Draw fitted rectangle.
        
        Args:
            color: Optional. (b,g,r) tuple.
            image: Optional. If set, writes over image; else, writes over a copy of self.img.         
        
        Returns:
            img_circle: image with drawn circle.
        '''
        # original image is modified by cv2.drawContours
        img_rect = image
        if img_rect is None:
            img_rect = self.img.copy()
            
        ### Not in opencv 2.x implement this function ###
        #box = cv2.boxPoints(self.rect)
        #box = np.int0(box)
        ###
        cv2.drawContours(im_rect,[box],0,color,2)
        
        return img_rect
    
    def fit_circle(self):
        '''Try to fit a circle.'''
        
        self.circ_pos, self.circ_radius = (0.0, 0.0), 0.0
        # Select contour
        for cnt in self.contours:
            # Now we try to fit a circle
            pos, rad = cv2.minEnclosingCircle(cnt)
            if rad > self.circ_radius:
                self.circ_pos, self.circ_radius = pos, rad
            circ_x, circ_y = self.circ_pos
            logging.getLogger(__name__).info("".join(("Circle. Pos:",str((circ_x, circ_y))," Radius:", str(self.circ_radius))))
    
    def fit_rectangle(self):
        '''Try to fit a rectangle.'''
        
        self.circ_pos, self.circ_radius = (0.0, 0.0), 0.0
        # Select contour
        if len(self.contours) > 0:
            M = cv2.moments(self.contours[0])
            if M['m00'] == 0:
                return
            self.centroid_x = int(M['m10']/M['m00'])
            self.centroid_y = int(M['m01']/M['m00'])
            self.rect = cv2.minAreaRect(self.contours[0])
            print "".join(("cx", str(self.centroid_x), " cy", str(self.centroid_y), " rect", str(self.rect)))
    
    @classmethod
    def boxPoints(cls, center, size, angle):
        '''Find the four vertices of a rotated rect. 
        
        Args:
            center: (x,y) coordinates of rectangle center.
            size: (width, height) of rectangle.
            angle: rotation angle in degrees.
        
        Returns:
            vertices: list of consecutive vertices.
            
        '''
        
        _angle = angle*np.pi/180.;
        b = np.cos(_angle)*0.5;
        a = np.sin(_angle)*0.5;
        
        v0 = (center[0] - a * size[1] - b * size[0], center[1] + b * size[1] - a * size[0])
        v1 = (center[0] + a * size[1] - b * size[0], center[1] - b * size[1] - a * size[0])
        v2 = (2 * center[0] - v0[0], 2 * center[1] - v0[1])
        v3 = (2 * center[0] - v1[0], 2 * center[1] - v1[1])
        
        vertices = [v0, v1, v2, v3]
        
        return vertices


if __name__ == "__main__":
    pass

