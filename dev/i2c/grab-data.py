#!/usr/bin/env python
# ----------------------------------------
# brew install libusb
# pip install pyftdi adafruit-blinka
# pip install adafruit-circuitpython-lsm6ds
# pip install adafruit-circuitpython-lps2x
# pip install adafruit-circuitpython-lis3mdl
# ----------------------------------------
# FT232H mac: /dev/tty.usbserial-1440
# ----------------------------------------
# camera: 0x33
# imu: 0x6A
# pressure: 0x5D

import os
if 'BLINKA_FT232H' in os.environ.keys():
    pass
else:
    os.environ['BLINKA_FT232H'] = "1"

import numpy as np
import time
from slurm.rate import Rate
from the_collector import BagIt, Pickle
from colorama import Fore
import board
import busio
from adafruit_lsm6ds import LSM6DS33,AccelRange,GyroRange
import adafruit_lps2x
import adafruit_lis3mdl
from math import pi


rad2deg = 180/pi
thumb = False
bag = BagIt(Pickle)
hz = 20
# rate = Rate(hz)

i2c = busio.I2C(board.SCL, board.SDA, 100000)

# 'RANGE_1000_DPS', 'RANGE_125_DPS', 'RANGE_2000_DPS', 'RANGE_250_DPS', 'RANGE_4000_DPS', 'RANGE_500_DPS'
imu = LSM6DS33(i2c)
imu.accelerometer_range = AccelRange.RANGE_2G  # pylint: disable=no-member
imu.gyro_range = GyroRange.RANGE_1000_DPS

lps = adafruit_lps2x.LPS22(i2c)
lis = adafruit_lis3mdl.LIS3MDL(i2c) # 155 Hz, 4 gauss, continuous
lis.data_rate = adafruit_lis3mdl.Rate.RATE_560_HZ

start = time.monotonic()
last = start
cnt = 0
m, p, t = 0, 0, 0

try:
    while True:
        # ts = time.time()
        # ts = time.monotonic()
        # dt = ts - last
        # mhz = int(1/dt)
        # last = ts

        m = [0,0,0]
        g = [0,0,0]
        a = [0,0,0]

        # a = imu.acceleration
        # g = imu.gyro; g = (g[0]*rad2deg, g[1]*rad2deg, g[2]*rad2deg,)
        m = lis.magnetic

        # if cnt%10 == 0:
        #     p = lps.pressure
        #     p = 145366.45 * (1 - (p/1013.25)**0.190284) * 0.3048
        #     # m = lis.magnetic
        #     t = lps.temperature
        #     # print(">")

        if cnt % (hz) == 0:
            # t = lps.temperature

            ts = time.monotonic()
            dt = ts - last
            mhz = int(cnt/dt)

            print(" "*80, end = "\r")
            print('{}Hz | {:>5.2f} {:>5.2f} {:>5.2f}g | {:>5.1f} {:>5.1f} {:>5.1f}uT | {:>4.1f} {:>4.1f} {:>4.1f}dps | {:>6.2f}m | {:>4.1f}C'.format(
                mhz,
                a[0], a[1], a[2],
                m[0], m[1], m[2],
                g[0], g[1], g[2],
                p,t),
                # end = "\r"
            )

            last = ts

        if cnt == hz:
            cnt = 1
        else:
            cnt += 1

        # rate.sleep()

except KeyboardInterrupt:
    print(">> bye ...")
    # bag.write("test", timestamp=False)
