##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################

# from math import sin, pi
import numpy as np
from numpy.linalg import norm
from numpy import cos, pi, sqrt
from numpy import sin, arcsin as asin
from numpy import arctan2, arctan as atan, tan


deg2rad = np.pi/180
rad2deg = 180/np.pi

class WGS84:
    """
    WGS84 is used in GPS which are geodetic coordinates.

    Geodetic coordinates are a type of curvilinear orthogonal coordinate
    system used in geodesy based on a reference ellipsoid. They include
    geodetic latitude (north/south) phi, longitude (east/west) lambda, and
    ellipsoidal height h (also known as geodetic height). The triad
    is also known as Earth ellipsoidal coordinates
    https://en.wikipedia.org/wiki/Geodetic_coordinates

    https://en.wikipedia.org/wiki/Earth_radius
    https://en.wikipedia.org/wiki/Earth_ellipsoid
    https://en.wikipedia.org/wiki/World_Geodetic_System#A_new_World_Geodetic_System:_WGS_84

    rf: inverse flattening
    f: flattening
    a: equitorial radius or semi-major axis [m]
    b: polar radius or semi-minor axis [m]
    e: eccentricity of Earth ellipsoid
    r: spherical radius [m]
    rotation_period: Earth's rotation rate [seconds]
    rate: Rotation rate of Earth [rad/s]
    sf: Schuller frequency
    """

    def __init__(self):
        self.rf = 298.257223563
        self.f = 1/self.rf
        self.a = 6378137.0
        self.b = self.a - self.a * self.f
        self.e = np.sqrt(1 - (self.b ** 2 / self.a ** 2))
        self.r = (2*self.a + self.b) / 3
        self.rotation_period = 23*3600 + 56*60 + 4.09053

        self.rate = 7.2921157e-5  # Rotation rate of Earth [rad/s]
        self.sf = 1.2383e-3       # Schuller frequency

    def gravity(self, lat):
        """
        Based off the Oxford reference for the gravity formula at sealevel.
        https://www.oxfordreference.com/view/10.1093/oi/authority.20110803100007626

        Also the WGS84 has a newer model, but it is more computationally
        intensive and only differs from this one by 0.68 um/s^2
        https://en.wikipedia.org/wiki/Gravity_of_Earth

        lat: latitude [decimal deg], North is positive and South is negative
        """
        lat = lat*deg2rad
        G0 = 9.7803253359              # Gravity [m/sec^2]
        return G0*(1 + 0.0053024*sin(lat)**2 - 0.0000058*sin(2*lat)**2)

    def haversine(self, a, b):
        """
        Returns the haversine (or great circle) distance between
        2 sets of GPS coordinates. This appears to work really well.

        a: (lat, lon) in deg
        b: (lat, lon) in deg
        """
        lat = b[0]*deg2rad
        # R = 6371008.8
        R = self.r
        dlat = (b[0] - a[0])*deg2rad
        dlon = (b[1] - a[1])*deg2rad
        m = sin(dlat*0.5)**2 + cos(a[0]*deg2rad) * cos(b[0]*deg2rad) * sin(dlon*0.5)**2
        return R*2*asin(min(1, sqrt(m)))

    def radius(self, lat):
        """
        Returns the geocentric radius based on WGS84

        lat: latitude in deg
        """
        a = self.a
        b = self.b
        lat = lat*deg2rad
        num = (a**2 * cos(lat))**2 + (b**2 * sin(lat))**2
        den = (a * cos(lat))**2 + (b * sin(lat))**2
        return sqrt(num / den)

    # def llh2ecef(self, lla):
    #     """
    #     llh: latitude (phi), longitude(lambda), height (or altitude) (H) in [deg, deg, m]
    #     ecef: Earth Centered Earth Fixed in [m, m, m]

    #     ref: https://en.wikipedia.org/wiki/Geographic_coordinate_conversion
    #     """
    #     lat = lla[0] *deg2rad
    #     lon = lla[1] * deg2rad

    #     if len(lla) == 2:
    #         H = 0
    #     else:
    #         H = lla[2]

    #     e2 = self.e**2
    #     n = self.a / sqrt(1.0 - e2 * sin(lat)**2)

    #     x = (n + H) * cos(lat) * cos(lon)
    #     y = (n + H) * cos(lat) * sin(lon)
    #     z = ((1 - e2) * n + H) * sin(lat)
    #     return np.array([x, y, z])

    def llh2ecef(self, lat, lon, H):
        # matlab: https://www.mathworks.com/help/aeroblks/llatoecefposition.html
        # this works, matches: https://www.oc.nps.edu/oc2902w/coord/llhxyz.htm
        mu = lat*pi/180
        i = lon*pi/180
        r = 6378137
        f = 1/298.257223563
        ls = atan((1-f)**2 * tan(mu))
        rs = sqrt(r**2 / (1+ (1/(1-f)**2 - 1)*sin(ls)**2))

        x = rs*cos(ls)*cos(i) + H*cos(mu)*cos(i)
        y = rs*cos(ls)*sin(i) + H*cos(mu)*sin(i)
        z = rs*sin(ls)+H*sin(mu)

        return x,y,z

    def ecef2llh(self, ecef):
        """
        llh: latitude (phi), longitude(lambda), height (or altitude) (H) in [deg, deg, m]
        ecef: Earth Centered Earth Fixed in [m, m, m]

        https://en.wikipedia.org/wiki/Geographic_coordinate_conversion#From_ECEF_to_geodetic_coordinates
        """
        x,y,z = ecef

        a = self.a
        a2 = self.a**2
        b2 = self.b**2
        e2 = self.e**2
        er2 = (a**2 - b2) / b2
        z2 = z**2

        p2 = x**2 + y**2
        p = sqrt(p2)

        F = 54 * b2 * z2
        G = p2 + (1-e2)*z2 - e2*(a2-b2)
        c = (e2**2 * F * p2) / G**3
        s = pow(1+c+sqrt(c**2 + 2*c), 1/3)
        k = s+1+1/s
        P = F / (3 * k**2 * G**2)
        Q = sqrt(1+ 2* e2**2 * P)
        ro = -(P * e2 * p) / (1 + Q) + sqrt(0.5*a2 * (1+1/Q) - P*(1-e2)*z2 / (Q+Q**2) - 0.5 * P *p2)
        U = sqrt((p-e2*ro)**2 + z2)
        V = sqrt((p-e2*ro)**2 + (1-e2) * z2)
        zo = b2*z / (a*V)
        h = U * (1 - b2 / (a*V))
        lat = arctan2(z+er2*zo, p) *rad2deg
        lon = arctan2(y, x) * rad2deg

        return np.array([lat, lon, h])



#---------------------------------------------------------------------

# RE = 6378137.0                 # Semi major axis of Earth [m]
# model = 'WGS84'
# FLATTENING = 0.00335281066475  # 1/298.257223563
# E2 = 0.00669437999014          # Eccentricity of Earth ellipsoid squared
# RATE = 7.2921157e-5            # Rotation rate of Earth [rad/s]
# SF = 1.2383e-3                 # Schuller frequency
# MU = 3.986004418e14            # Gravitational parameter of Earth
# G0 = 9.7803253359              # Gravity [m/sec^2]
# # gravity = 9.81                 # Grabity [m/sec^2]

# def gravity(lat):
#     """
#     Based off the Oxford reference for the gravity formula at sealevel.
#     https://www.oxfordreference.com/view/10.1093/oi/authority.20110803100007626

#     Also:
#     https://en.wikipedia.org/wiki/Gravity_of_Earth

#     lat: latitude [decimal deg], North is posative and South is negative
#     """
#     lat *= pi/180
#     return G0*(1 + 0.0053024*sin(lat)**2 - 0.0000058*sin(2*lat)**2)

# deg2rad = np.pi/180
# rad2deg = 180/np.pi