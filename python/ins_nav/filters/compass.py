##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################
# import attr
from math import cos, sin, pi, atan2, asin, sqrt
from squaternion import Quaternion
from ins_nav.utils import rad2deg
from ins_nav.utils import normalize3
from enum import IntFlag

Angle = IntFlag("Angle", "degrees radians quaternion")

# @attr.s(slots=True)
class TiltCompensatedCompass(object):
    """
    A tilt compensated compass is basically just taking the magnetometer
    readings and adjusting them based of how the sensor is oriented (tilted).
    We use the accelerometer to determine the orientation relative to the
    ground. Obviously, if the sensor is under some sort of gravity loading,
    say movng fast (falling), then this won't work correctly.

    Also, the in sensor inputs are expected to have already been adjusted
    for biases and other issues (hard/soft iron errors).
    """

    # angle_units = attr.ib(default=Angle.degrees)

    def __init__(self, angle_units=Angle.degrees):
        """
        angle_units: degrees, radians, quaternion
        quaternion: default is (1,0,0,0), but you can set it to something else
        """
        self.angle_units = angle_units

    def compensate(self, accel, mag):
        """

        """

        try:
            mx, my, mz = normalize3(*mag)
            ax, ay, az = normalize3(*accel)

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

            if self.angle_units == Angle.degrees:
                roll    *= rad2deg
                pitch   *= rad2deg
                heading *= rad2deg
            elif self.angle_units == Angle.quaternion:
                return Quaternion.from_euler(roll, pitch, heading)

            return (roll, pitch, heading,)

        except ZeroDivisionError as e:
            print('Error', e)
            if self.angle_units == Angle.quaternion:
                return Quaternion(1, 0, 0, 0)
            else:
                return (0.0, 0.0, 0.0,)
