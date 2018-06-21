from __future__ import print_function
from __future__ import division
import numpy as np
from math import sin, cos, atan2, pi, sqrt, asin
from math import radians as deg2rad
from math import degrees as rad2deg

def normalize(x, y, z):
	"""Return a unit vector"""
	norm = sqrt(x * x + y * y + z * z)
	if norm > 0.0:  # why did I do this?
		inorm = 1/norm  # why did I do this?
		x *= inorm
		y *= inorm
		z *= inorm
	else:
		raise Exception('division by zero: {} {} {}'.format(x, y, z))
	return (x, y, z)

def getOrientation(accel, mag, deg=True):
	ax, ay, az = normalize(*accel)
	mx, my, mz = normalize(*mag)

	roll = atan2(ay, az)
	pitch = atan2(-ax, ay*sin(roll)+az*cos(roll))

	heading = atan2(
		mz*sin(roll) - my*cos(roll),
		mx*cos(pitch) + my*sin(pitch)*sin(roll) + mz*sin(pitch)*cos(roll)
	)

	if deg:
		roll *= 180/pi
		pitch *= 180/pi
		heading *= 180/pi

		heading = heading if heading >= 0.0 else 360 + heading
		heading = heading if heading <= 360 else heading - 360
	else:
		heading = heading if heading >= 0.0 else 2*pi + heading
		heading = heading if heading <= 2*pi else heading - 2*pi

	return (roll, pitch, heading)
