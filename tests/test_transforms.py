"""
https://www.mathworks.com/help/aerotbx/ug/ecef2lla.html
ecef2lla([4510731 4510731 0]) => 0   45.0000  999.9564
ecef2lla([0 4507609 4498719], 'WGS84') => 45.1358   90.0000  999.8659

https://www.mathworks.com/help/aerotbx/ug/lla2ecef.html
lla2ecef([0 45 1000]) => 1.0e+006 * 4.5107    4.5107         0
lla2ecef([45 90 2000], 'WGS84') => 1e6 * 0.0000    4.5190    4.4888

Can also double check with https://www.ngs.noaa.gov/NCAT/
"""
from ins_nav.transforms import ecef2llh, llh2ecef
from ins_nav.transforms import NED, ENU
from math import pi, sqrt
import numpy as np
from numpy.linalg import norm


data = [
    # ecef [m,m,m]    lat,lon,alt [deg,deg,m]
    # ((5.057590377e6, 2.694861463e6, -2.794229000e6), (-26.145055, 28.050255, 1660.86)), # Pretoria S. Africa
    # ((-4.646678571e6, 2.549341033e6, -3.536478881e6), (-33.89115, 151.249186, 86.26)),  # Sydney Australia
    ((4510731, 4510731, 0), (0, 45, 999.9564)),      # matlab
    # ((0.0000, 4.5190e6, 4.4888e6), (45, 90, 2000)),  # matlab
]

def test_ecef2llh():
    for e, ans in data:
        f = ecef2llh(*e)
        assert norm(f - np.array(ans)) < 0.01, f"ecef2llh {f - ans}"


def test_llh2ecef():
    for ans, ll in data:
        f = llh2ecef(*ll)
        assert norm(f - np.array(ans)) < 0.01, f"llh2ecef {f - ans}"

def test_ned():
    n = NED.from_ll(0,45, 999.9564)
    nn = NED.from_ecef(4510731, 4510731, 0)

    assert norm(n.origin - nn.origin) < 1
    assert norm(n.R - nn.R) < 0.000001
    assert n.type == nn.type

    # print(n.origin)
    # print(nn.origin)
    # print(n.R)
    # print(n.R - nn.R)
