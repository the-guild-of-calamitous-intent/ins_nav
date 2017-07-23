from __future__ import print_function
from __future__ import division
# import numpy as np
from math import sin, cos, atan2, pi, sqrt, asin
from math import radians as deg2rad
from math import degrees as rad2deg
# from collections import namedtuple
#
# Quaternion = namedtuple('Quaternion', 'w x y z')


def normalize(x, y, z):
	"""Return a unit vector"""
	norm = sqrt(x * x + y * y + z * z)

	# already a unit vector
	if norm == 1.0:
		return (x, y, z)

	if norm > 0.0:
		inorm = 1/norm
		x *= inorm
		y *= inorm
		z *= inorm
	else:
		raise Exception('division by zero: {} {} {}'.format(x, y, z))
	return (x, y, z)


def normalize_q(q):
	"""Return a unit quaternion"""
	norm = sqrt(q[0]**2 + q[1]**2 + q[2]**2 + q[3]**2)

	# already a unit quaternion
	if norm == 1.0:
		return q

	if norm > 0:
		inorm = 1/norm
		qo = [x*inorm for x in q]
	else:
		raise Exception('normalize div by 0')
	return qo


def quat2euler(q):
	"""
	Returns the euler representation (in degrees) of a quaternion. Note, the
	heading is wrapped between 0-360 degrees.

	In:
		[q0 q1 q2 q3] = [w x y z]
	out:
		(roll, pitch, yaw) in degrees
	"""
	q0, q1, q2, q3 = q
	roll = atan2(2.0*q2*q3-2.0*q0*q1, 2.0*q0*q0+2.0*q3*q3-1.0)
	pitch = -asin(2.0*q1*q3+2.0*q0*q2)
	heading = atan2(2.0*q1*q2-2.0*q0*q3, 2.0*q0*q0+2.0*q1*q1-1.0)

	heading = heading if heading <= 2*pi else heading-2*pi
	heading = heading if heading >= 0 else heading+2*pi

	return map(rad2deg, (roll, pitch, heading))
