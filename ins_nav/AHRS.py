#!/usr/bin/env python

from __future__ import print_function
from __future__ import division
# from math import atan2, sin, cos, pi
from math import sqrt
from .utils import normalize, normalize_q
from math import radians as deg2rad
from math import degrees as rad2deg


class AHRS(object):
	def __init__(self, q=None):
		if q is None:
			q = (1, 0, 0, 0)
		q = normalize_q(q)
		self.q = q

	def updateAGM(self, a, m, g, beta, dt):
		"""
		q - current quaternion
		a - acceleration [g's], this will be normalize
		m - magnetometer readings [uT], this will be normalized
		g - gyro readings [rad/sec]
		beta - function of sensor noise
		dt - time step [sec]
		"""
		q0, q1, q2, q3 = self.q
		gx, gy, gz = (deg2rad(x) for x in g)
		ax, ay, az = a
		mx, my, mz = m

		# Rate of change of quaternion from gyroscope
		qDot1 = 0.5 * (-q1 * gx - q2 * gy - q3 * gz)
		qDot2 = 0.5 * (q0 * gx + q2 * gz - q3 * gy)
		qDot3 = 0.5 * (q0 * gy - q1 * gz + q3 * gx)
		qDot4 = 0.5 * (q0 * gz + q1 * gy - q2 * gx)

		ax, ay, az = normalize(ax, ay, az)
		mx, my, mz = normalize(mx, my, mz)

		# Auxiliary variables to avoid repeated arithmetic
		_2q0mx = 2.0 * q0 * mx
		_2q0my = 2.0 * q0 * my
		_2q0mz = 2.0 * q0 * mz
		_2q1mx = 2.0 * q1 * mx
		_2q0 = 2.0 * q0
		_2q1 = 2.0 * q1
		_2q2 = 2.0 * q2
		_2q3 = 2.0 * q3
		_2q0q2 = 2.0 * q0 * q2
		_2q2q3 = 2.0 * q2 * q3
		q0q0 = q0 * q0
		q0q1 = q0 * q1
		q0q2 = q0 * q2
		q0q3 = q0 * q3
		q1q1 = q1 * q1
		q1q2 = q1 * q2
		q1q3 = q1 * q3
		q2q2 = q2 * q2
		q2q3 = q2 * q3
		q3q3 = q3 * q3

		# Reference direction of Earth's magnetic field
		hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3
		hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3
		_2bx = sqrt(hx * hx + hy * hy)
		_2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3
		_4bx = 2.0 * _2bx
		_4bz = 2.0 * _2bz

		# Gradient decent algorithm corrective step
		s0 = -_2q2 * (2.0 * q1q3 - _2q0q2 - ax) + _2q1 * (2.0 * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz)
		s1 = _2q3 * (2.0 * q1q3 - _2q0q2 - ax) + _2q0 * (2.0 * q0q1 + _2q2q3 - ay) - 4.0 * q1 * (1 - 2.0 * q1q1 - 2.0 * q2q2 - az) + _2bz * q3 * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz)
		s2 = -_2q0 * (2.0 * q1q3 - _2q0q2 - ax) + _2q3 * (2.0 * q0q1 + _2q2q3 - ay) - 4.0 * q2 * (1 - 2.0 * q1q1 - 2.0 * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz)
		s3 = _2q1 * (2.0 * q1q3 - _2q0q2 - ax) + _2q2 * (2.0 * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz)

		s0, s1, s2, s3 = normalize_q((s0, s1, s2, s3))

		# Apply feedback step
		qDot1 -= beta * s0
		qDot2 -= beta * s1
		qDot3 -= beta * s2
		qDot4 -= beta * s3

		q0 += qDot1 * dt
		q1 += qDot2 * dt
		q2 += qDot3 * dt
		q3 += qDot4 * dt

		q0, q1, q2, q3 = normalize_q((q0, q1, q2, q3))

		self.q = (q0, q1, q2, q3)

		return (q0, q1, q2, q3)

	def updateAG(self, q, a, g, beta, dt):
		q0, q1, q2, q3 = q
		gx, gy, gz = (deg2rad(x) for x in g)
		ax, ay, az = a

		# Rate of change of quaternion from gyroscope
		qDot1 = 0.5 * (-q1 * gx - q2 * gy - q3 * gz)
		qDot2 = 0.5 * (q0 * gx + q2 * gz - q3 * gy)
		qDot3 = 0.5 * (q0 * gy - q1 * gz + q3 * gx)
		qDot4 = 0.5 * (q0 * gz + q1 * gy - q2 * gx)

		# Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
		ax, ay, az = normalize(ax, ay, az)

		# Auxiliary variables to avoid repeated arithmetic
		_2q0 = 2.0 * q0
		_2q1 = 2.0 * q1
		_2q2 = 2.0 * q2
		_2q3 = 2.0 * q3
		_4q0 = 4.0 * q0
		_4q1 = 4.0 * q1
		_4q2 = 4.0 * q2
		_8q1 = 8.0 * q1
		_8q2 = 8.0 * q2
		q0q0 = q0 * q0
		q1q1 = q1 * q1
		q2q2 = q2 * q2
		q3q3 = q3 * q3

		# Gradient decent algorithm corrective step
		s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay
		s1 = _4q1 * q3q3 - _2q3 * ax + 4.0 * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az
		s2 = 4.0 * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az
		s3 = 4.0 * q1q1 * q3 - _2q1 * ax + 4.0 * q2q2 * q3 - _2q2 * ay

		s0, s1, s2, s3 = normalize_q((s0, s1, s2, s3))

		# Apply feedback step
		qDot1 -= beta * s0
		qDot2 -= beta * s1
		qDot3 -= beta * s2
		qDot4 -= beta * s3

		# Integrate rate of change of quaternion to yield quaternion
		q0 += qDot1 * dt
		q1 += qDot2 * dt
		q2 += qDot3 * dt
		q3 += qDot4 * dt

		q0, q1, q2, q3 = normalize_q((q0, q1, q2, q3))

		self.q = (q0, q1, q2, q3)

		return (q0, q1, q2, q3)


# class AHRS(object):
# 	# roll: Rotation around the X-axis. -180 <= roll <= 180
# 	# a positive roll angle is defined to be a clockwise rotation about the positive X-axis
# 	#
# 	#                    y
# 	#      roll = atan2(---)
# 	#                    z
# 	#
# 	# where:  y, z are returned value from accelerometer sensor
# 	#
# 	# pitch: Rotation around the Y-axis. -180 <= roll <= 180
# 	# a positive pitch angle is defined to be a clockwise rotation about the positive Y-axis
# 	#
# 	#                                 -x
# 	#      pitch = atan(-------------------------------)
# 	#                    y * sin(roll) + z * cos(roll)
# 	#
# 	# where:  x, y, z are returned value from accelerometer sensor
# 	#
# 	# heading: Rotation around the Z-axis. -180 <= roll <= 180
# 	# a positive heading angle is defined to be a clockwise rotation about the positive Z-axis
# 	#
# 	#                                       z * sin(roll) - y * cos(roll)
# 	#   heading = atan2(--------------------------------------------------------------------------)
# 	#                    x * cos(pitch) + y * sin(pitch) * sin(roll) + z * sin(pitch) * cos(roll))
# 	#
# 	# where:  x, y, z are returned value from magnetometer sensor
#
# 	def __init__(self, deg=False):
# 		self.deg = deg
#
# 	def getOrientation(self, raw_accel, mag):
# 		accel = (raw_accel)
# 		ax, ay, az = accel
# 		mx, my, mz = mag
# 		roll = atan2(ay, az)
# 		pitch = atan2(-ax, ay*sin(roll)+az*cos(roll))
#
# 		heading = atan2(
# 			mz*sin(roll) - my*cos(roll),
# 			mx*cos(pitch) + my*sin(pitch)*sin(roll) + mz*sin(pitch)*cos(roll)
# 		)
#
# 		if self.deg:
# 			roll *= 180/pi
# 			pitch *= 180/pi
# 			heading *= 180/pi
#
# 			heading = heading if heading >= 0.0 else 360 + heading
# 			heading = heading if heading <= 360 else heading - 360
# 		else:
# 			heading = heading if heading >= 0.0 else 2*pi + heading
# 			heading = heading if heading <= 2*pi else heading - 2*pi
#
# 		return (roll, pitch, heading)
