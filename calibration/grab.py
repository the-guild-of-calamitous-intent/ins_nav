#!/usr/bin/env python
# 6 July 2019 Kevin Walchko, MIT License
# Grabs IMU data and stores it to a bag file

from nxp_imu import IMU
import time
from the_collector import BagIt, Pickle
from collections import namedtuple

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

class Rate(object):
    """
    Uses sleep to keep a desired message/sample rate.
    """
    def __init__(self, hertz):
        self.last_time = time.time()
        self.dt = 1/hertz

    def sleep(self):
        """
        This uses sleep to delay the function. If your loop is faster than your
        desired Hertz, then this will calculate the time difference so sleep
        keeps you close to you desired hertz. If your loop takes longer than
        your desired hertz, then it doesn't sleep.
        """
        now = time.time()
        diff = now - self.last_time
        if diff < self.dt:
            new_sleep = self.dt - diff
            time.sleep(new_sleep)

        # now that we hav slept a while, set the current time
        # as the last time
        self.last_time = time.time()

Data = namedtuple('Data', 'data timestamp')

if __name__ == "__main__":
    bag = BagIt(Pickle)

    # Open the NXP IMU with:
    #  accel: 2 g's
    #  gyros: 250 degrees per sec
    imu = IMU(gs=2, dps=250)

    rate = Rate(200)

    try:
        for i in range(10000):
        # while True:
            a, m, g = imu.get()
            ts = time.time()
            bag.push('accel', Data(a,ts))
            bag.push('mag', Data(m,ts))
            bag.push('gyro', Data(g,ts))

            # pts = lidar.read()
            # bag.push('lidar', pts)

            # print("[{}] {}".format(i, g))
            # print(type(g))
            if i%20 == 0:
                print(i)

            rate.sleep()

    except Exception as e:
        print(e)
    except KeyboardInterrupt:
        pass

    # bag.write('data-still.bag')
    bag.write('data', timestamp=False)
    # lidar.close()
    print('Done ...')
