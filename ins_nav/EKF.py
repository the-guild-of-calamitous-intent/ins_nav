import numpy as np
# from numpy import cross
from numpy import dot
import numdifftools as nd
from numpy.linalg import inv
from pyrk import RK4


class EKF(object):
	"""
	Extended Kalman Filter (EKF)

	def func(time, x, u):
		some nonlinear eqns
		return dx

	ekf = EKF(size_x,size_z)
	ekf.init(x, func, R, Q)

	while True:
		ekf.predict(u)
		x_hat = ekf.update(z)
	"""
	def __init__(self, dim_x, dim_z, dt):
		# self.x = x  # init state
		# self.rk = RK4(f)
		self.dt = dt
		self.F = np.eye(dim_x)
		self.H = 0
		self.P = np.eye(dim_x)
		self.R = np.eye(dim_z)
		self.Q = np.eye(dim_x)
		self.I = np.eye(dim_x)

	def init(self, x, f, r, q):
		"""
		f - dx = f(x, u)
		r -
		q -
		"""
		self.x = x
		self.rk = RK4(f)
		self.jacob = nd.Jacobian(f)
		self.R = r
		self.Q = q
		self.time = 0.0

	def something(self, u):
		rk = self.rk
		dt = self.dt
		t = self.time
		x = self.x

		y = rk.step(x, u, t, dt)
		self.F = self.jacob(y)

		self.time = t+dt
		self.x = y

		return y

	def predict(self, u):
		self.x = self.something(u)

		F = self.F
		Q = self.Q
		P = self.P

		self.P = dot(F, dot(P, F.T)) + Q

	def update(self, z):
		H = self.H
		R = self.R
		I = self.I
		P = self.P
		x = self.x

		K = dot(P, dot(H.T, inv(dot(H, dot(P, H.T)) + R)))
		x = x + K.dot(z-H)
		p = (I - K.dot(H)).dot(P)

		self.x = x
		self.P = p

		return x
