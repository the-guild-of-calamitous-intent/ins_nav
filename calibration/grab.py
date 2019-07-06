#!/usr/bin/env python

from __future__ import division, print_function
from nxp_imu import IMU
# from compass import Compass
from pydar import LDS01
from the_collector import BagWriter
# import time
import platform

system = platform.system()

# lidar serial port
if system == 'Darwin':
    port = "/dev/tty.SLAB_USBtoUART"
elif system == 'Linux':
    # also valid
    # /dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0
    port = "/dev/ttyUSB0"
else:
    raise Exception("unsupported OS")

"""
nxp_imu
accel/mag - 0x1f
gyro - 0x21
pi@r2d2 nxp $ sudo i2cdetect -y 1
    0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 1f
20: -- 21 -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- --
"""


if __name__ == "__main__":
    bag = BagWriter()
    # bag.open(['lidar', 'accel', 'mag', 'grav'])
    # bag.use_compression = True

    imu = IMU(gs=4, dps=2000, verbose=True)

    lidar = LDS01()
    lidar.open(port)
    lidar.run(True)  # why true?

    try:
        for i in range(500):
        # while True:
            a, m, g = imu.get()
            bag.push('accel', list(a))
            bag.push('mag', list(m))
            bag.push('gyro', list(g))

            pts = lidar.read()
            bag.push('lidar', pts)

            # print("[{}] {}".format(i, g))
            # print(type(g))
            if i%20 == 0:
                print(i)

    except Exception as e:
        print(e)
    except KeyboardInterrupt:
        pass

    # bag.write('data-still.bag')
    bag.write('data-moving-2.bag')
    lidar.close()
    print('Done ...')
