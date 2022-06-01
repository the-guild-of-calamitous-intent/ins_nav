##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################
from math import sqrt
from math import pi
# from math import cos, sin
import ins_nav.wgs84

RAD2DEG = 180/pi
DEG2RAD = pi/180
rad2deg = 180/pi
deg2rad = pi/180
FT2M = 0.3048   # feet to meters
MI2M = 1609.34  # miles to meters

def normalize3(x, y, z):
    """Return a unit vector"""
    norm = sqrt(x * x + y * y + z * z)

    # already a unit vector
    if norm == 1.0:
        return (x, y, z)

    inorm = 1.0/norm
    if inorm > 1e-6:
        x *= inorm
        y *= inorm
        z *= inorm
    else:
        raise ZeroDivisionError(f'norm({x:.4f}, {y:.4f}, {z:.4f},) = {inorm:.6f}')
    return (x, y, z,)
