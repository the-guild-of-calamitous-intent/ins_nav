[![](https://raw.githubusercontent.com/MomsFriendlyRobotCompany/ins_nav/master/docs/pics/header.jpg)](https://github.com/MomsFriendlyRobotCompany/ins_nav)

# ins_nav

[![Actions Status](https://github.com/MomsFriendlyRobotCompany/ins_nav/workflows/pytest/badge.svg)](https://github.com/MomsFriendlyRobotCompany/ins_nav/actions)
![PyPI - License](https://img.shields.io/pypi/l/ins_nav.svg)
![PyPI - Python Version](https://img.shields.io/pypi/pyversions/ins_nav.svg)
![PyPI - Format](https://img.shields.io/pypi/format/ins_nav.svg)
![PyPI](https://img.shields.io/pypi/v/ins_nav.svg)


This library is written independent of any specific IMU. The idea is you pass in the appropriate
measurements and error terms from your IMU and get the desired output.

**This is still under heavy development**

## Reference Frames

* [ECI: Earth-centered Inertial](https://en.wikipedia.org/wiki/Earth-centered_inertial) is an
inertial frame where Newton's laws of motion apply. It has its origin at the center of the
Earth with:
    - x-axis in the direction of the vernal equinox
    - z-axis is parallel to the rotation of the Earth
    - y-axis completes the right-handed coordinate system
* [ECEF: Earth-centered, Earth-fixed](https://en.wikipedia.org/wiki/ECEF) has the same origin
as ECI, but rotates with the Earth and the x-axis points towards the zero/prime
meridian. The ECEF frame rotates at 7.2921E-5 rads/sec with respect to the ECI
frame
* [LLA(H): Latitude, Longitude, Altitude(Height)](tbd) is similar to the ECEF frame, but
is the frame historically used for GPS navigation

## WGS84

```python
wgs = WGS84()

# here are a bunch of useful constants in WGS84 class
wgs.rf = 298.257223563
wgs.f = 1/self.rf
wgs.a = 6378137.0
wgs.b = self.a - self.a * self.f
wgs.e = np.sqrt(1 - (self.b ** 2 / self.a ** 2))
wgs.r = (2*self.a + self.b) / 3
wgs.rotation_period = 23*3600 + 56*60 + 4.09053
wgs.rate = 7.2921157e-5  # Rotation rate of Earth [rad/s]
wgs.sf = 1.2383e-3       # Schuller frequency

# translate ecef(x,y,z) <=> llh(lat,lon,alt)
wgs.ecef2llh(loc)
wgs.llh2ecef(loc)

wgs.gravity(lat)   # gravity changes by latitude[deg]
wgs.radius(lat)    # Earth's radius changes by latitude[deg]
wgs.haversine(a,b) # calculates distance between locations a and b
```

## Navigation Frames

While ECEF can be used to navigate the globe, often, you only need to travel 100's of meters
or kilometers. Thus a local navigational frame that is tangental to the curvature of the
Earth is more useful (and intuitive). Two common ones are:

* [ENU: East North Up](https://en.wikipedia.org/wiki/Axes_conventions#Ground_reference_frames:_ENU_and_NED)
a local navigation frame, where *up* and the z-axis align, but clockwise right turns
are negative
* [NED: North East Down](https://en.wikipedia.org/wiki/North_east_down) a local navigation
frame, where *up* and the z-axis are opposite, but the direction of right (clockwise)
turns are in the positive direction and is the standard vehicle roll-pitch-yaw frame

```python
ref = (40,-90,100) # origin of local frame (lat[deg], lon[deg], altitude[m])
frame = NavigationalFrame(ref)

# allof these will calculate position relative to your local
# frame origin
loc = [x,y,z]
frame.ecef2ned(loc)
frame.ecef2enu(loc)
frame.ned2ecef(loc)
frame.enu2ecef(loc)
frame.enu2ned(loc)
frame.ned2enu(loc)
```

# Other Good Navigation Libraries

- [lat_lon_parser](https://pypi.org/project/lat-lon-parser/) allows you to convert between
measurements formats like `-45 deg 12' 36.0 sec`, `45.21 W`, and `-45.21` easily
- [nvector](https://www.navlab.net/nvector) has a lot of capability
- [navpy](https://github.com/NavPy/NavPy) appears to be simple grad student work but code is well referenced (BSD)
- [navigation](https://github.com/ngfgrant/navigation) does GPS navigation and way
points

# The MIT License (MIT)

**Copyright (c) 2016 Kevin J. Walchko**

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
