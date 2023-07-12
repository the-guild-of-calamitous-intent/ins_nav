[![](https://raw.githubusercontent.com/MomsFriendlyRobotCompany/ins_nav/master/docs/pics/header.jpg)](https://github.com/MomsFriendlyRobotCompany/ins_nav)

# `ins_nav`

This library is written independent of any specific IMU. The idea is you pass in the appropriate
measurements and error terms from your IMU and get the desired output.

**This is still under heavy development**

## Software

- `WGS84`
    - Holds properties of the Earth
    - Converts between ECEF (GPS) and LLH
    - Haversine
- Filters
    - `TiltCompensatedCompass`
    - `Madgwick`
    - `Mahony`
- Storage (Python only)
    - `to_yaml`
    - `to_pickle`
- Calibration (Python only)
    - See `docs/jupyter`

## Global Reference Frames

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

## Local Navigation Frames

While ECEF can be used to navigate the globe, often, you only need to travel 100's of meters
or kilometers. Thus a local navigational frame that is tangental to the curvature of the
Earth is more useful (and intuitive). Two common ones are:

* [ENU: East North Up](https://en.wikipedia.org/wiki/Axes_conventions#Ground_reference_frames:_ENU_and_NED)
a local navigation frame, where *up* and the z-axis align, but clockwise right turns
are negative
* [NED: North East Down](https://en.wikipedia.org/wiki/North_east_down) a local navigation
frame, where *up* and the z-axis are opposite, but the direction of right (clockwise)
turns are in the positive direction and is the standard vehicle roll-pitch-yaw frame

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
