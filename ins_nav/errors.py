from math import sin, sqrt, pi
from ins_nav import earth


def calc_errors(bias, mis, arw, time):
    """
    https://www.vectornav.com/support/library/imu-and-ins
    bias = bias [g's]
    mis = axis missalignment [degs]
    ARW = angular random walk [deg/sart(hour)]
    time = duration of data capture [sec]
    """
    g = earth.G0
    ebias = 0.5*g*bias*time**2
    emis = 0.5*g*sin(mis*pi/180)*time**2
    earw = 0.5*g*sin(arw*sqrt(time/3600))*time**2
    
    return (ebias, emis, earw,)
