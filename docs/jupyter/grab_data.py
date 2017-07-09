#!/usr/bin/env python

from __future__ import division, print_function
# from nxp_imu import FXAS21002, FXOS8700
# from nxp_imu.FXAS21002 import GYRO_RANGE_250DPS
# from nxp_imu import I2C
from nxp_imu import IMU
# from nxp_imu import AHRS
import time
import simplejson as json


# def print_data(data):
# 	print("| {:20} | {:20} |".format("Accels [g's]", "Orient(r, p, h) [deg]"))
# 	print('-'*47)
# 	for a in data['accel']:
# 		# a, m, g = imu.get()
# 		# r, p, h = ahrs.getOrientation(a, m)
# 		print('| {:>6.2f} {:>6.2f} {:>6.2f} | {:>6.2f} {:>6.2f} {:>6.2f} |'.format(a[0], a[1], a[2], r, p, h))


if __name__ == "__main__":
	try:
		imu = IMU()
		# ahrs = AHRS(True)
		data = {
			'accel': [],
			'mag': [],
			'gyro': [],
		}

		while True:
			a, m, g = imu.get()
			data['accel'].append(a)
			data['mag'].append(m)
			data['gyro'].append(g)
			time.sleep(0.1)

	except KeyboardInterrupt:
		with open('data.json', 'w') as f:
			json.dump(data, f)
		print('saved 3x{} data points'.format(len(data['accel'])))
