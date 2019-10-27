##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################
from ins_nav.wgs84 import RE, FLATTENING, E2
from math import sqrt, atan2, sin, cos
from ins_nav.utils import RAD2DEG, DEG2RAD
import numpy as np


# New ------------------------------------------------------------------------------
def ecef2llh(x, y, z):
    """
    ecef: Earth Centered Earth Fixed in [m, m, m]
    llh: latitude, longitude, height (or altitude) in [deg, deg, m]
    """
    p = sqrt(x**2 + y**2)
    b = RE*(1-FLATTENING)
    ep = (RE**2 - b**2)/(b**2)
    theta = atan2(z*RE, p*b)

    l = atan2(y, x)
    L = atan2(z+ep*b*sin(theta)**3, p-E2*RE*cos(theta)**3)
    Re = RE/sqrt(1-E2*sin(L)**2)
    h = p/cos(L) - Re

    L = RAD2DEG * L
    l = RAD2DEG * l
    return (L, l, h,)


def llh2ecef(lat, lon, H):
    """
    ecef: Earth Centered Earth Fixed in [m, m, m]
    llh: latitude, longitude, height (or altitude) in [deg, deg, m]

    phi = lat
    lambda = lon
    H = height or altitude
    """
    lat = DEG2RAD * lat
    lon = DEG2RAD * lon
    e2 = E2 # 0.00669437999014
    re = RE # 6378137.0  # m
    rm = re * (1.0 - e2) / pow(1.0 - e2 * sin(lat)**2, 3.0 / 2.0)
    rn = re / sqrt(1.0 - e2 * sin(lat)**2)
    x = (rn + H) * cos(lat) * cos(lon)
    y = (rn + H) * cos(lat) * sin(lon)
    z = (rm + H) * sin(lat)
    return (x, y, z,)


def llh2DCM(lat, lon, h, w):
    # lat/lon [rads]
    # h = height [m]
    # w = wander azmuth
    sL = sin(lat)
    cL = cos(lat)
    sl = sin(lon)
    cl = cos(lon)
    Cgn = np.array([[w[1], w[0], 0], [-w[0], w[1], 0], [0, 0, 1]])
    Ceg = np.array([[-sL*cl, -sL*sl, cL], [-sl, cl, 0], [-cL*cl, -cL*sl, -sL]])
    return Cgn.dot(Ceg)

#------------------------------------------------------------------------------------
