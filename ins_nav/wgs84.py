##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################

from math import sin

RE = 6378137.0                 # Semi major axis of Earth [m]
model = 'WGS84'
FLATTENING = 0.00335281066475  # 1/298.257223563
E2 = 0.00669437999014          # Eccentricity of Earth ellipsoid squared
RATE = 7.2921157e-5            # Rotation rate of Earth [rad/s]
SF = 1.2383e-3                 # Schuller frequency
MU = 3.986004418e14            # Gravitational parameter of Earth
G0 = 9.7803253359              # Gravity [m/sec^2]
# gravity = 9.81                 # Grabity [m/sec^2]

def gravity(lat):
    """
    Based off the Oxford reference for the gravity formula at sealevel.
    https://www.oxfordreference.com/view/10.1093/oi/authority.20110803100007626

    Also:
    https://en.wikipedia.org/wiki/Gravity_of_Earth

    lat: latitude [decimal deg], North is posative and South is negative
    """
    lat *= pi/180
    return G0*(1 + 0.0053024*sin(lat)**2 - 0.0000058*sin(2*lat)**2)

# value?
# class WGS84:
#     re = 6378137.0  # m
#     FLATTENING = 0.00335281066475
#     E2 = 0.00669437999014
