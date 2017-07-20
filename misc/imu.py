

class IMU(object):
	ACCEL = 1
	GYRO = 2
	MAG = 4

	def __init__(self):
		pass

	def __del__(self):
		pass

	def read(self, sensors=7):
		"""
		Accel = m/sec^2
		Gyros = deg/sec
		Magnet = Tesla ?
		"""
		a = None  # change to None
		g = None
		m = None
		if sensors & 1:
			a = (0, 0.1, .9)  # read accel
		if sensors & 2:
			g = (0, 0, 0)  # read gyros
		if sensors & 4:
			m = (1, 1, 1)  # read mag
		return (a, g, m)

	def __str__(self):
		a, g, m = self.read()
		return 'A: {} G: {} M: {}'.format(a, g, m)
