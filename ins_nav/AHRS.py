#!/usr/bin/env python

from __future__ import print_function
from __future__ import division
from math import atan2, sin, cos, pi


class AHRS(object):
	# roll: Rotation around the X-axis. -180 <= roll <= 180
	# a positive roll angle is defined to be a clockwise rotation about the positive X-axis
	#
	#                    y
	#      roll = atan2(---)
	#                    z
	#
	# where:  y, z are returned value from accelerometer sensor
	#
	# pitch: Rotation around the Y-axis. -180 <= roll <= 180
	# a positive pitch angle is defined to be a clockwise rotation about the positive Y-axis
	#
	#                                 -x
	#      pitch = atan(-------------------------------)
	#                    y * sin(roll) + z * cos(roll)
	#
	# where:  x, y, z are returned value from accelerometer sensor
	#
	# heading: Rotation around the Z-axis. -180 <= roll <= 180
	# a positive heading angle is defined to be a clockwise rotation about the positive Z-axis
	#
	#                                       z * sin(roll) - y * cos(roll)
	#   heading = atan2(--------------------------------------------------------------------------)
	#                    x * cos(pitch) + y * sin(pitch) * sin(roll) + z * sin(pitch) * cos(roll))
	#
	# where:  x, y, z are returned value from magnetometer sensor

	def __init__(self, deg=False):
		self.deg = deg

	def getOrientation(self, raw_accel, mag):
		accel = (raw_accel)
		ax, ay, az = accel
		mx, my, mz = mag
		roll = atan2(ay, az)
		pitch = atan2(-ax, ay*sin(roll)+az*cos(roll))

		heading = atan2(
			mz*sin(roll) - my*cos(roll),
			mx*cos(pitch) + my*sin(pitch)*sin(roll) + mz*sin(pitch)*cos(roll)
		)

		if self.deg:
			roll *= 180/pi
			pitch *= 180/pi
			heading *= 180/pi

			heading = heading if heading >= 0.0 else 360 + heading
			heading = heading if heading <= 360 else heading - 360
		else:
			heading = heading if heading >= 0.0 else 2*pi + heading
			heading = heading if heading <= 2*pi else heading - 2*pi

		return (roll, pitch, heading)
