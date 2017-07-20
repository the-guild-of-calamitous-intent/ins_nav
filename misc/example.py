#!/usr/bin/env python

from navigation import AHRS
from navigation import IMU

try:
	from Adafruit_LSM303 import LSM303
except ImportError:
	from fake_rpi import LSM303


class MyIMU(IMU):
	def __init__(self):
		# MinIMU-9 (L3G4200D and LSM303DLM carrier)
		# http://www.pololu.com/catalog/product/1265
		# accel and mag are measured at 12b

		# pi@zoidberg pygecko $ sudo i2cdetect -y 1
		# 0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
		# 00:          -- -- -- -- -- -- -- -- -- -- -- -- --
		# 10: -- -- -- -- -- -- -- -- 18 -- -- -- -- -- 1e --
		# 20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
		# 30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
		# 40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
		# 50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
		# 60: -- -- -- -- -- -- -- -- -- 69 -- -- -- -- -- --
		# 70: -- -- -- -- -- -- -- --
		# 18 [24]  - accel
		# 1e [30]  - magnometer
		# 69 [105] - gyros

		self.lsm303 = LSM303(accel_address=0x18, mag_address=0x1e)
		self.lsm303._mag.write8(0x01, 0x20)  # set mag to 1.3 gauss
		self.lsm303._mag.write8(0x23, 0x00)  # set accel to 2 g


if __name__ == "__main__":
	imu = MyIMU()
	ahrs = AHRS(imu, angle_units=AHRS.DEGREES)
	print('Heading [deg]: {:.2f} {:.2f} {:.2f}'.format(*ahrs.read()))
