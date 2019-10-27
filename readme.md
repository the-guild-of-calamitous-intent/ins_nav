[![](https://raw.githubusercontent.com/MomsFriendlyRobotCompany/ins_nav/master/docs/pics/header.jpg)](https://github.com/MomsFriendlyRobotCompany/ins_nav)

# ins_nav

![PyPI - License](https://img.shields.io/pypi/l/ins_nav.svg)
![PyPI - Python Version](https://img.shields.io/pypi/pyversions/ins_nav.svg)
![PyPI - Format](https://img.shields.io/pypi/format/ins_nav.svg)
![PyPI](https://img.shields.io/pypi/v/ins_nav.svg)


This library is written independent of any specific IMU. The idea is you pass in the appropriate
measurements and error terms from your IMU and get the desired output.

**This is still under heavy development**

# Install

The suggested way to install this is via the `pip` command as follows::

```
pip install ins_nav
```

## Development

To submit git pulls, clone the repository and set it up as follows::

```
git clone https://github.com/MomsFriendlyRobotCompany/ins_nav
cd ins_nav
pip install -e .
```

## Usage

- `ins_nav.wgs84` contains a bunch of useful constants: semi-major axis, gravity, etc
- `TiltCompensatedCompass` contains the mathematics of an IMU with accelerometers, gyroscopes, and magnetometers
- `ins_nav.transforms` has a bunch of reference frame conversions: `ecef2llh`, `llh2ecef`, etc
- `ins_nav.errors` has functions to esitmate the horizontal errors given an IMU's bias, mis-alignment, and random walk noise
- `ins_nav.ahrs` creates an attitude and heading reference system (AHRS) using accelerometers, gyroscopes, and magnetometers

## Transforms

* [ECEF: Earth-centered, Earth-fixed](https://en.wikipedia.org/wiki/ECEF)
* [ECI: Earth-centered Inertial](https://en.wikipedia.org/wiki/Earth-centered_inertial)
* [ENU: East North Up](https://en.wikipedia.org/wiki/Axes_conventions#Ground_reference_frames:_ENU_and_NED)
* [NED: North East Down](https://en.wikipedia.org/wiki/North_east_down)


## Todo

- extended kalman filter
- navigation equations
- error model

# Change Log

||||
|------------|---------|-------------------------|
| 2019-07-05 | 0.5.1   | cleanup and new functions|
| 2017-07-07 | 0.0.1   | init                    |


# The MIT License (MIT)

Copyright (c) 2017 Kevin J. Walchko

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
