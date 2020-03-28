##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################
from math import sqrt
from math import pi
from math import cos, sin
import ins_nav.wgs84
#
# Quaternion = namedtuple('Quaternion', 'w x y z')

RAD2DEG = 180/pi
DEG2RAD = pi/180
FT2M = 0.3048   # feet to meters
MI2M = 1609.34  # miles to meters

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
        raise ZeroDivisionError('division by zero: {} {} {}'.format(x, y, z))
    return (x, y, z,)
