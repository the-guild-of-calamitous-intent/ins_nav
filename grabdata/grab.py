#!/usr/bin/env python

from __future__ import division, print_function
from nxp_imu import IMU
# from compass import Compass
from pydar import LDS01
from the_collector import BagWriter
import time

"""
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


# def imu():
# 	imu = IMU(gs=4, dps=2000, verbose=True)
# 	header = 67
# 	print('-'*header)
# 	print("| {:17} | {:20} | {:20} |".format("Accels [g's]", " Magnet [uT]", "Gyros [dps]"))
# 	print('-'*header)
# 	for _ in range(10):
# 		a, m, g = imu.get()
# 		print('| {:>5.2f} {:>5.2f} {:>5.2f} | {:>6.1f} {:>6.1f} {:>6.1f} | {:>6.1f} {:>6.1f} {:>6.1f} |'.format(
# 			a[0], a[1], a[2],
# 			m[0], m[1], m[2],
# 			g[0], g[1], g[2])
# 		)
# 		time.sleep(0.50)
# 	print('-'*header)
# 	print(' uT: micro Tesla')
# 	print('  g: gravity')
# 	print('dps: degrees per second')
# 	print('')


# def ahrs():
# 	print('')
# 	imu = IMU(verbose=True)
# 	header = 47
# 	print('-'*header)
# 	print("| {:20} | {:20} |".format("Accels [g's]", "Orient(r,p,h) [deg]"))
# 	print('-'*header)
# 	for _ in range(10):
# 		a, m, g = imu.get()
# 		r, p, h = imu.getOrientation(a, m)
# 		print('| {:>6.1f} {:>6.1f} {:>6.1f} | {:>6.1f} {:>6.1f} {:>6.1f} |'.format(a[0], a[1], a[2], r, p, h))
# 		time.sleep(0.50)
# 	print('-'*header)
# 	print('  r: roll')
# 	print('  p: pitch')
# 	print('  h: heading')
# 	print('  g: gravity')
# 	print('deg: degree')
# 	print('')


if __name__ == "__main__":
	bag = BagWriter()
	bag.open(['lidar', 'accel', 'mag', 'grav'])
	bag.use_compression = True

	imu = IMU(gs=4, dps=2000, verbose=True)

	lidar = LDS01()
	lidar.open("/dev/tty.SLAB_USBtoUART")  # is it always this? based on os?
	lidar.run(True)  # why true?

	try:
		while True:
			a, m, g = imu.get()
			bag.push('accel', a)
			bag.push('mag', m)
			bag.push('grav', g)

			pts = lidar.read()
			bag.push('lidar', pts)

	except Exception as e:
		print(e)
	except KeyboardInterrupt:
		pass

	bag.write('data.json')
	lidar.close()
	print('Done ...')
