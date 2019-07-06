#!/usr/bin/env python
##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################

from __future__ import print_function
from __future__ import division
# import os              # check we are not on travis.ci
# import platform        # determine linux or darwin (OSX)
from math import cos, sin, pi, atan2, asin, sqrt
from quaternions import Quaternion


# if platform.system().lower() == 'linux' and 'CI' not in os.environ:
try:
	from Adafruit_LSM303 import LSM303
# else:
except ImportError:
	import random

	print('WARNING: Using fake LSM303 (compass) library')

	class HW(object):
		def write8(self, a, b):
			pass

	class LSM303(object):
		"""
		Dummy interface for testing outside of linux/RPi where I don't have
		access to I2C and the real sensor. Also, check to see if we are on
		travis.ci which also doesn't have i2c access but is linux.
		"""
		def __init__(self, accel_address, mag_address):
			random.seed()  # init for random data
			self._mag = HW()

		def read(self):
			"""
			Since there isn't a real sensor connected, read() creates random
			data.
			"""
			data = []
			for i in range(6):
				data.append(random.uniform(-2048, 2048))
			# accel = AHRS.normalize(*data[:3])
			# mag = AHRS.normalize(*data[3:])
			accel = data[:3]
			mag = data[3:]
			return accel, mag


class AHRS(object):
	def __init__(self):
		# MinIMU-9 (L3G4200D and LSM303DLM carrier)
		# http://www.pololu.com/catalog/product/1265
		# accel and mag are measured at 12b

		# Mx points to North

		# pi@zoidberg pygecko $ sudo i2cdetect -y 1
		# 0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
		# 00:          -- -- -- -- -- -- -- -- -- -- -- -- --
		# 10: -- -- -- -- -- -- -- -- 18 -- -- -- -- -- 1e --
		# 20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
		# 30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
		# 40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
		# 50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
		# 60: -- -- -- -- -- -- -- -- -- 69 -- -- -- -- -- --
		# 70: -- -- -- -- -- -- -- --
		# 18 [24]  - accel
		# 1e [30]  - magnometer
		# 69 [105] - gyros

		self.lsm303 = LSM303(accel_address=0x18, mag_address=0x1e)
		self.lsm303._mag.write8(0x01, 0x20)  # set mag to 1.3 gauss
		self.lsm303._mag.write8(0x23, 0x00)  # set accel to 2 g

	@staticmethod
	def normalize(a, b, c):
		m = sqrt(a**2 + b**2 + c**2)

		if m < 1e-6:
			raise Exception('normalize: div by zero')

		a /= m
		b /= m
		c /= m

		return (a, b, c)

	def quaterion(self):
		r, p, y = self.read()
		return Quaternion.from_eluer(r, p, y)

	@staticmethod
	def grav(x, y, z):
		# converts to g's
		# default is 2 g's
		div = 2048/2.0
		x /= div
		y /= div
		z /= div
		return (x, y, z)

	@staticmethod
	def mag(x, y, z):
		# converts to gauss
		# default is 1.3 gauss
		div = 2048/1.3
		x /= div
		y /= div
		z /= div
		return (x, y, z)

	def read(self, deg=False):
		"""
		heading is fucked up but roll/pitch seem to be ok
		"""
		accel, mag = self.lsm303.read()

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

		if deg:
			roll    *= 180/pi
			pitch   *= 180/pi
			heading *= 180/pi

		return roll, pitch, heading


if __name__ == "__main__":
	ahrs = AHRS()
	print('Heading [deg]: {:.2f} {:.2f} {:.2f}'.format(*ahrs.read(deg=True)))
