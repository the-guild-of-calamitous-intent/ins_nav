import numpy as np
from numpy.linalg import norm
from squaternion import Quaternion
from math import sqrt, pi


class MARG:
    def __init__(self, B, Z):
        self.wb = np.array([0,0,0])
        self.q = Quaternion()
        self.wcomp = True
        self.B = B
        self.zeta = Z
        self.qwe = Quaternion(0,0,0,0)
        self.bias = np.array([0.,0.,0.])
        self.M = np.eye(3)

    def comp(self, del_f, dt):
        q = self.q
        qwe = 2.0*q.conjugate*del_f # 47
        self.qwe = self.qwe + qwe.normalize*dt
        self.wb = self.wb + self.zeta*np.array(self.qwe[1:]) # 48
        return self.wb

    def calc_b(self,m):
        # wiki: v' = q.v.q*
        q = self.q
        h = q*Quaternion(0,m[0],m[1],m[2])*q.conjugate # 45
        return (sqrt(h.x**2+h.y**2),0,h.z)             # 46

    def grad(self,a,m):
        ax,ay,az = a
        mx,my,mz = m
        bx,by,bz = self.calc_b(m) # calc Earth magnetic field reference
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
        ]) # 26

        fb = np.array([
            [2*bx*(0.5-q3**2-q4**2)+2*bz*(q2*q4-q1*q3)-mx],
            [    2*bx*(q2*q3-q1*q4)+2*bz*(q1*q2+q3*q4)-my],
            [2*bx*(q1*q3+q2*q4)+2*bz*(0.5-q2**2-q3**2)-mz]
        ]) # 29
        Jb = np.array([
            [        -2*bz*q3,         2*bz*q4, -4*bx*q3-2*bz*q1, -4*bx*q4+2*bz*q2],
            [-2*bx*q4+2*bz*q2, 2*bx*q3+2*bz*q1,  2*bx*q2+2*bz*q4, -2*bx*q1+2*bz*q3],
            [         2*bx*q3, 2*bx*q4-4*bz*q2,  2*bx*q1-4*bz*q3,          2*bx*q2]
        ]) # 30

        aa = np.vstack((Jg,Jb))  # [6x4]
        bb = np.vstack((fg,fb))  # [6x1]
        cc = (aa.T @ bb).ravel() # [4x6]*[6x1] = [4x1]
        self.q = Quaternion(*cc).normalize
        return self.q

    def update(self,a,g,m,dt):
        a = a/norm(a)
        m = self.M @ (m - self.bias)
        w = g
        q = self.q

        del_f = self.grad(a,m)   # gradient decent algorithm

        if self.wcomp:
            wb = self.comp(del_f,dt) # calculate bias
            w = w - wb               # 49

        qdw = 0.5*q*Quaternion(0,*w) #
        qdot = qdw - self.B*del_f # filter
        q = q + qdot*dt    # integrate
        self.q = q.normalize
        return self.q
