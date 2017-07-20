#!/usr/bin/env python

##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################

from __future__ import print_function
from __future__ import division
from math import cos, sin, pi, atan2, asin, sqrt
from quaternions import Quaternion
from imu import IMU


class AHRS(object):
	DEGREES = 1
	RADIANS = 2
	QUATERNIONS = 4

	def __init__(self, imu, angle_units=DEGREES):
		# Mx points to North
		self.imu = imu
		self.angle_units = angle_units

	@staticmethod
	def normalize(a, b, c):
		m = sqrt(a**2 + b**2 + c**2)

		if m < 1e-6:
			raise ZeroDivisionError('AHRS::normalize(): div by near zero [1e-6]')

		a /= m
		b /= m
		c /= m

		return (a, b, c)

	def quaterion(self):
		r, p, y = self.read()  # don't like this ... save last reading? input rpy?
		return Quaternion.from_eluer(r, p, y)

	def read(self):
		"""

		"""
		accel, _, mag = self.imu.read(sensors=(IMU.ACCEL | IMU.MAG))

		# print(accel,  mag)

		try:

			mx, my, mz = self.normalize(*mag)
			# ax, ay, az = self.grav(*accel)
			ax, ay, az = self.normalize(*accel)
			# print('accel {:.4f} {:.4f} {:.4f}\t\tmag {:.4f} {:.4f} {:.4f}'.format(ax, ay, az, mx, my, mz))

			pitch = asin(-ax)

			if abs(pitch) >= pi/2:
				roll = 0.0
			else:
				roll = asin(ay/cos(pitch))

			# mx, my, mz = mag
			x = mx*cos(pitch)+mz*sin(pitch)
			y = mx*sin(roll)*sin(pitch)+my*cos(roll)-mz*sin(roll)*cos(pitch)
			heading = atan2(y, x)

			# wrap heading between 0 and 360 degrees
			if heading > 2*pi:
				heading -= 2*pi
			elif heading < 0:
				heading += 2*pi

			if self.angle_units == self.DEGREES:
				roll    *= 180/pi
				pitch   *= 180/pi
				heading *= 180/pi
			elif self.angle_units == self.QUATERNIONS:
				return Quaternion.from_eluer(roll, pitch, heading)

			return (roll, pitch, heading)

		except ZeroDivisionError as e:
			print('Error', e)
			return (0, 0, 0)
