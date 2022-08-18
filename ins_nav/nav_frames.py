##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################

# from math import sin, pi
import numpy as np
from numpy.linalg import norm
from numpy import arcsin, cos, sin, pi, sqrt, arcsin as asin, arctan2

deg2rad = np.pi/180
rad2deg = 180/np.pi

class NavigationFrame:
    """
    Converts between ENU <-> ECEF <-> NED

    All of these frames are useful for navigation either globally (ECEF)
    or locally (NED, ENU).

    Geodetic coordinates: latitude, longitude which are reported by GPS

    ECEF: a geocentric coordinate system, which is just a 3D cartesian
          spatial reference system with the origin at the Earth's
          center of mass.

    ref: https://en.wikipedia.org/wiki/Earth-centered,_Earth-fixed_coordinate_system
    """

    def __init__(self, llref):
        """
        llref: (lat, lon) of reference frame origin, a third element
                altitude can also be included (lat, lon, alt) otherwise
                the altitude is assumed to be 0.
        """
        self.latr = llref[0]*deg2rad
        self.lonr = llref[1]*deg2rad
        self.ecefr = self.__llh2ecef(llref)

        # this matrix converts ned <-> enu
        # notice R == R.T, so no need to transpose it
        self.R = np.array([
            [0,1,0],
            [1,0,0],
            [0,0,-1]
        ])

    def __llh2ecef(self, lla):
        """
        Geodetic coordinates are a type of curvilinear orthogonal coordinate
        system used in geodesy based on a reference ellipsoid. They include
        geodetic latitude (north/south) phi, longitude (east/west) lambda, and
        ellipsoidal height h (also known as geodetic height). The triad
        is also known as Earth ellipsoidal coordinates
        https://en.wikipedia.org/wiki/Geodetic_coordinates

        llh: latitude (phi), longitude(lambda), height (or altitude) (H) in [deg, deg, m]
        ecef: Earth Centered Earth Fixed in [m, m, m]

        ref: https://en.wikipedia.org/wiki/Geographic_coordinate_conversion
        """
        lat = lla[0] *deg2rad
        lon = lla[1] * deg2rad

        if len(lla) == 2:
            H = 0
        else:
            H = lla[2]

        a  = 6378137.0        # Semi major axis of Earth [m]
        e2 = 0.00669437999014 # Eccentricity of Earth ellipsoid squared
        n = a / sqrt(1.0 - e2 * sin(lat)**2)

        x = (n + H) * cos(lat) * cos(lon)
        y = (n + H) * cos(lat) * sin(lon)
        z = ((1 - e2) * n + H) * sin(lat)

        return np.array([x, y, z])

    def ecef2enu(self, ecef):
        """
        ref: https://en.wikipedia.org/wiki/Geographic_coordinate_conversion#From_ECEF_to_ENU
        """
        lat = self.latr
        lon = self.lonr
        ref = self.ecefr

        slat = sin(lat); clat = cos(lat)
        slon = sin(lon); clon = cos(lon)

        # FIXME: get rid of trig calls
        r = np.array([
            [-sin(lon),                    cos(lon),        0],
            [-sin(lat)*cos(lon), -sin(lat)*sin(lon), cos(lat)],
            [ cos(lat)*cos(lon),  cos(lat)*sin(lon), sin(lat)]
        ])

        return r.dot(ecef - ref)

    def enu2ecef(self, enu):
        """
        ref: https://en.wikipedia.org/wiki/Geographic_coordinate_conversion#From_ENU_to_ECEF
        """
        lat = self.latr
        lon = self.lonr
        ref = self.ecefr

        slat = sin(lat); clat = cos(lat)
        slon = sin(lon); clon = cos(lon)

        r = np.array([
            [-slon, -slat*clon, clat*clon],
            [ clat, -slat*slon, clat*slon],
            [    0,       clat,      slat]
        ])

        return r.dot(enu) + ref

    def ecef2ned(self, ecef):
        """
        llh: latitude (phi), longitude(lambda), height (or altitude) (H) in [deg, deg, m]
        ecef: Earth Centered Earth Fixed in [m, m, m]
        ned: North, East, Down in [m, m, m]

        ref: https://en.wikipedia.org/wiki/Local_tangent_plane_coordinates
        """
        lat = self.latr
        lon = self.lonr
        ref = self.ecefr

        slat = sin(lat); clat = cos(lat)
        slon = sin(lon); clon = cos(lon)

        #     R = np.array(
        #         [
        #             [-np.sin(lat)*np.cos(lon), -np.sin(lat)*np.sin(lon), np.cos(lat)],
        #             [-np.sin(lon), np.cos(lon), 0],
        #             [-np.cos(lat)*np.cos(lon), -np.cos(lat)*np.sin(lon), -np.sin(lat)]
        #         ]
        #     )

        # FIXME: get rid of transpose
        r = np.array([
            [-slat*clon, -slon, -clat*clon],
            [-slat*slon,  clon, -clat*slon],
            [      clat,     0,      -slat]
        ]).T

        return r.dot(ecef - ref)

    def ned2ecef(self, ned):
        """
        llh: latitude (phi), longitude(lambda), height (or altitude) (H) in [deg, deg, m]
        ecef: Earth Centered Earth Fixed in [m, m, m]
        ned: North, East, Down in [m, m, m]

        ref: https://en.wikipedia.org/wiki/Local_tangent_plane_coordinates
        """
        lat = self.latr
        lon = self.lonr
        ref = self.ecefr

        slat = sin(lat); clat = cos(lat)
        slon = sin(lon); clon = cos(lon)

        r = np.array([
            [-slat*clon, -slon, -clat*clon],
            [-slat*slon,  clon, -clat*slon],
            [      clat,     0,      -slat]
        ])

        return r.dot(ned) + ref

    def ned2enu(self, ned):
        return self.R.dot(ned)

    def enu2ned(self, enu):
        return self.R.dot(enu)

