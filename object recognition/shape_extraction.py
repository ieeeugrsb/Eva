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
    
    def fit_ractangle(self):
    '''Try to fit a rectangle.'''
        
        self.circ_pos, self.circ_radius = (0.0, 0.0), 0.0
        # Select contour
        for cnt in self.contours:
            # Now we try to fit a circle
            pos, rad = cv2.minEnclosingCircle(cnt)
            if rad > self.circ_radius:
                self.circ_pos, self.circ_radius = pos, rad
            circ_x, circ_y = self.circ_pos
            logging.getLogger(__name__).info("".join(("Circle. Pos:",str((circ_x, circ_y))," Radius:", str(self.circ_radius))))



if __name__ == "__main__":
    pass

