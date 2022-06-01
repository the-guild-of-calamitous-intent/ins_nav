import numpy as np
from numpy.linalg import norm
from squaternion import Quaternion
from math import sqrt, pi


class IMUFilter:
    """
    ref: Madgwick, An efficient orientation filter for inertial
        and inertial/magnetic sensor arrays
    """
    bias = None
    mat = None

    def __init__(self, B, q=None):
        """
        q: [optional] initial quaternion, otherwise [1,0,0,0]
        B: Beta term in Madgwick's paper based on filter gradient learning
        """
        self.wb = np.array([0,0,0])
        self.wcomp = True # estimate gyro bias
        self.wdr = 0.2 # gyro drift rate deg/sec/sec

        if q is None:
            self.q = Quaternion()
        else:
            self.q = q

        self.B = B

    def comp(self, del_f, dt):
        """
        Does the gyro compensation
        """
        q = self.q
        zeta = sqrt(3/4)*self.wdr*pi/180 # gyro drift: rad/s/s
        qwe = 2.0*q.conjugate*del_f # 47
        qwe = qwe.normalize
        self.wb = self.wb + zeta*np.array(qwe[1:])*dt # 48
        return self.wb

    def grad(self, a):
        """
        Calculates eqn #34, del_f/norm(del_f), which is the top
        block in the IMU flow diagram.
        """
        ax,ay,az = a
        q1,q2,q3,q4 = self.q

        fg = np.array([
            [2*(q2*q4-q1*q3)-ax],
            [2*(q1*q2+q3*q4)-ay],
            [2*(0.5-q2**2-q3**2)-az]
        ]) # 25
        Jg = np.array([
            [-2*q3,  2*q4, -2*q1, 2*q2],
            [ 2*q2,  2*q1,  2*q4, 2*q3],
            [    0, -4*q2, -4*q3,    0]
        ]) # 26, gradient of fg

        d = (Jg.T @ fg).T #[0]
        d = d.ravel()
        qq = Quaternion(*d).normalize
        return qq.normalize

    def update(self, a, w, dt):
        """
        a: acceleration, term is normalize, so can be in any units
        w: gyro rates [rads/sec]
        dt: time step [sec]
        """
        a = a/norm(a)
        q = self.q
        del_f = self.grad(a) # gradient decent algorithm

        if self.wcomp:
            wb = self.comp(del_f,dt) # calculate bias
            w = w - wb               # 49

        qdw = 0.5*q*Quaternion(0,*w)
        qdot = qdw - self.B*del_f # filter
        q = q + qdot*dt           # integrate
        self.q = q.normalize
        return self.q
