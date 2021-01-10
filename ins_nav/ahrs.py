##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################
import attr
from math import sqrt
import numpy as np # matrix manipulations
from numpy.linalg import norm
# from ins_nav.utils import normalize3
# from math import radians as deg2rad
# from math import degrees as rad2deg
from squaternion import Quaternion
from ins_nav.utils import RAD2DEG, DEG2RAD


class IMUFilter:
    """
    ref: Madgwick, An efficient orientation filter for inertial
        and inertial/magnetic sensor arrays
    """
    def __init__(self, B, q=None):
        """
        q: [optional] initial quaternion, otherwise [1,0,0,0]
        B: Beta term in Madgwick's paper based on filter gradient learning
        """
        self.wb = np.array([0,0,0])
        self.wcomp = True # estimate gyro bias
        self.wdr = 0.2 # gyro drift rate deg/sec/sec

        if q is None:
            self.q = Quaternion()
        else:
            self.q = q

        self.B = B

    def comp(self, del_f, dt):
        """
        Does the gyro compensation
        """
        q = self.q
        zeta = sqrt(3/4)*self.wdr*pi/180 # gyro drift: rad/s/s
        qwe = 2.0*q.conjugate*del_f # 47
        qwe = qwe.normalize
        self.wb = self.wb + zeta*np.array(qwe[1:])*dt # 48
        return self.wb

    def grad(self, a):
        """
        Calculates eqn #34, del_f/norm(del_f), which is the top
        block in the IMU flow diagram.
        """
        ax,ay,az = a
        q1,q2,q3,q4 = self.q

        fg = np.array([
            [2*(q2*q4-q1*q3)-ax],
            [2*(q1*q2+q3*q4)-ay],
            [2*(0.5-q2**2-q3**2)-az]
        ]) # 25
        Jg = np.array([
            [-2*q3,  2*q4, -2*q1, 2*q2],
            [ 2*q2,  2*q1,  2*q4, 2*q3],
            [    0, -4*q2, -4*q3,    0]
        ]) # 26, gradient of fg

        d = (Jg.T @ fg).T #[0]
        d = d.ravel()
        qq = Quaternion(*d).normalize
        return qq.normalize

    def update(self, a, w, dt):
        """
        a: acceleration, term is normalize, so can be in any units
        w: gyro rates [rads/sec]
        dt: time step [sec]
        """
        a = a/norm(a)
        q = self.q
        del_f = self.grad(a) # gradient decent algorithm

        if self.wcomp:
            wb = self.comp(del_f,dt) # calculate bias
            w = w - wb               # 49

        qdw = 0.5*q*Quaternion(0,*w)
        qdot = qdw - self.B*del_f # filter
        q = q + qdot*dt           # integrate
        self.q = q.normalize
        return self.q







# @attr.s(slots=True)
# class AHRS(object):
#     q = attr.ib(default=Quaternion())
#
#     def reset(self, q=None):
#         if q is None:
#             self.q = Quaternion(1, 0, 0, 0)
#         else:
#             self.q = q.normalize
#
#     def updateAGM(self, a, m, g, beta, dt, degrees=True):
#         """
#         Internally, the current orientation (self.q) is tracked:
#         q - current quaternion Quaternion(w,x,y,z)
#
#         Args:
#         a - acceleration [g's], this will be normalize
#         m - magnetometer readings [uT], this will be normalized
#         g - gyro readings [rad/sec]
#         beta - function of sensor noise
#         dt - time step [sec]
#         degrees - if True, convert to rads/sec
#
#         Return:
#         q - current quaternion Quaternion(w,x,y,z)
#         """
#         q0, q1, q2, q3 = self.q
#
#         if degrees:
#             g = (x*DEG2RAD for x in g)
#         gx, gy, gz = g
#         ax, ay, az = a
#         mx, my, mz = m
#
#         # Rate of change of quaternion from gyroscope
#         qDot1 = 0.5 * (-q1 * gx - q2 * gy - q3 * gz)
#         qDot2 = 0.5 * ( q0 * gx + q2 * gz - q3 * gy)
#         qDot3 = 0.5 * ( q0 * gy - q1 * gz + q3 * gx)
#         qDot4 = 0.5 * ( q0 * gz + q1 * gy - q2 * gx)
#
#         ax, ay, az = normalize3(ax, ay, az)
#         mx, my, mz = normalize3(mx, my, mz)
#
#         # Auxiliary variables to avoid repeated arithmetic
#         _2q0mx = 2.0 * q0 * mx
#         _2q0my = 2.0 * q0 * my
#         _2q0mz = 2.0 * q0 * mz
#         _2q1mx = 2.0 * q1 * mx
#         _2q0 = 2.0 * q0
#         _2q1 = 2.0 * q1
#         _2q2 = 2.0 * q2
#         _2q3 = 2.0 * q3
#         _2q0q2 = 2.0 * q0 * q2
#         _2q2q3 = 2.0 * q2 * q3
#         q0q0 = q0 * q0
#         q0q1 = q0 * q1
#         q0q2 = q0 * q2
#         q0q3 = q0 * q3
#         q1q1 = q1 * q1
#         q1q2 = q1 * q2
#         q1q3 = q1 * q3
#         q2q2 = q2 * q2
#         q2q3 = q2 * q3
#         q3q3 = q3 * q3
#
#         # Reference direction of Earth's magnetic field
#         hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3
#         hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3
#         _2bx = sqrt(hx * hx + hy * hy)
#         _2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3
#         _4bx = 2.0 * _2bx
#         _4bz = 2.0 * _2bz
#
#         # Gradient decent algorithm corrective step
#         s0 = -_2q2 * (2.0 * q1q3 - _2q0q2 - ax) + _2q1 * (2.0 * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz)
#         s1 = _2q3 * (2.0 * q1q3 - _2q0q2 - ax) + _2q0 * (2.0 * q0q1 + _2q2q3 - ay) - 4.0 * q1 * (1 - 2.0 * q1q1 - 2.0 * q2q2 - az) + _2bz * q3 * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz)
#         s2 = -_2q0 * (2.0 * q1q3 - _2q0q2 - ax) + _2q3 * (2.0 * q0q1 + _2q2q3 - ay) - 4.0 * q2 * (1 - 2.0 * q1q1 - 2.0 * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz)
#         s3 = _2q1 * (2.0 * q1q3 - _2q0q2 - ax) + _2q2 * (2.0 * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz)
#
#         # print(">>",s0, s1, s2, s3)
#         s0, s1, s2, s3 = Quaternion(s0, s1, s2, s3).normalize
#
#         # Apply feedback step
#         qDot1 -= beta * s0
#         qDot2 -= beta * s1
#         qDot3 -= beta * s2
#         qDot4 -= beta * s3
#
#         q0 += qDot1 * dt
#         q1 += qDot2 * dt
#         q2 += qDot3 * dt
#         q3 += qDot4 * dt
#
#         self.q = Quaternion(q0, q1, q2, q3).normalize
#
#         return self.q
#
#     def updateAG(self, a, g, beta, dt, degrees=True):
#         q0, q1, q2, q3 = self.q
#
#         if degrees:
#             g = (x*DEG2RAD for x in g)
#
#         gx, gy, gz = g
#         ax, ay, az = a
#
#         # Rate of change of quaternion from gyroscope
#         qDot1 = 0.5 * (-q1 * gx - q2 * gy - q3 * gz)
#         qDot2 = 0.5 * (q0 * gx + q2 * gz - q3 * gy)
#         qDot3 = 0.5 * (q0 * gy - q1 * gz + q3 * gx)
#         qDot4 = 0.5 * (q0 * gz + q1 * gy - q2 * gx)
#
#         # Compute feedback only if accelerometer measurement valid (avoids NaN
#         # in accelerometer normalisation)
#         ax, ay, az = normalize3(ax, ay, az)
#
#         # Auxiliary variables to avoid repeated arithmetic
#         _2q0 = 2.0 * q0
#         _2q1 = 2.0 * q1
#         _2q2 = 2.0 * q2
#         _2q3 = 2.0 * q3
#         _4q0 = 4.0 * q0
#         _4q1 = 4.0 * q1
#         _4q2 = 4.0 * q2
#         _8q1 = 8.0 * q1
#         _8q2 = 8.0 * q2
#         q0q0 = q0 * q0
#         q1q1 = q1 * q1
#         q2q2 = q2 * q2
#         q3q3 = q3 * q3
#
#         # Gradient decent algorithm corrective step
#         s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay
#         s1 = _4q1 * q3q3 - _2q3 * ax + 4.0 * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az
#         s2 = 4.0 * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az
#         s3 = 4.0 * q1q1 * q3 - _2q1 * ax + 4.0 * q2q2 * q3 - _2q2 * ay
#
#         s0, s1, s2, s3 = Quaternion(s0, s1, s2, s3).normalize
#
#         # Apply feedback step
#         qDot1 -= beta * s0
#         qDot2 -= beta * s1
#         qDot3 -= beta * s2
#         qDot4 -= beta * s3
#
#         # Integrate rate of change of quaternion to yield quaternion
#         q0 += qDot1 * dt
#         q1 += qDot2 * dt
#         q2 += qDot3 * dt
#         q3 += qDot4 * dt
#
#         self.q = Quaternion(q0, q1, q2, q3).normalize
#
#         return self.q


# class AHRS(object):
#     # roll: Rotation around the X-axis. -180 <= roll <= 180
#     # a positive roll angle is defined to be a clockwise rotation about the positive X-axis
#     #
#     #                    y
#     #      roll = atan2(---)
#     #                    z
#     #
#     # where:  y, z are returned value from accelerometer sensor
#     #
#     # pitch: Rotation around the Y-axis. -180 <= roll <= 180
#     # a positive pitch angle is defined to be a clockwise rotation about the positive Y-axis
#     #
#     #                                 -x
#     #      pitch = atan(-------------------------------)
#     #                    y * sin(roll) + z * cos(roll)
#     #
#     # where:  x, y, z are returned value from accelerometer sensor
#     #
#     # heading: Rotation around the Z-axis. -180 <= roll <= 180
#     # a positive heading angle is defined to be a clockwise rotation about the positive Z-axis
#     #
#     #                                       z * sin(roll) - y * cos(roll)
#     #   heading = atan2(--------------------------------------------------------------------------)
#     #                    x * cos(pitch) + y * sin(pitch) * sin(roll) + z * sin(pitch) * cos(roll))
#     #
#     # where:  x, y, z are returned value from magnetometer sensor
#
#     def __init__(self, deg=False):
#         self.deg = deg
#
#     def getOrientation(self, raw_accel, mag):
#         accel = (raw_accel)
#         ax, ay, az = accel
#         mx, my, mz = mag
#         roll = atan2(ay, az)
#         pitch = atan2(-ax, ay*sin(roll)+az*cos(roll))
#
#         heading = atan2(
#             mz*sin(roll) - my*cos(roll),
#             mx*cos(pitch) + my*sin(pitch)*sin(roll) + mz*sin(pitch)*cos(roll)
#         )
#
#         if self.deg:
#             roll *= 180/pi
#             pitch *= 180/pi
#             heading *= 180/pi
#
#             heading = heading if heading >= 0.0 else 360 + heading
#             heading = heading if heading <= 360 else heading - 360
#         else:
#             heading = heading if heading >= 0.0 else 2*pi + heading
#             heading = heading if heading <= 2*pi else heading - 2*pi
#
#         return (roll, pitch, heading)
