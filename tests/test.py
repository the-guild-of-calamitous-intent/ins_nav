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
from math import pi, sqrt


def error(a, b):
    # calculate the norm error
    er = 0
    for i in range(3):
        er += (a[i] - b[i])**2
    return sqrt(er)


def test_ecef2llh():
    f = ecef2llh(4510731, 4510731, 0)
    ans = (0, 45.0000, 999.9564,)
    assert error(f, ans) < 0.001, "ecef2llh {} {}".format(f,ans)


def test_llh2ecef():
    f = llh2ecef(0, 45, 1000)
    ans = (4.510731*1e6, 4.510731*1e6, 0)
    assert error(f, ans) < 0.1, "llh2ecef {} {}".format(f,ans)


# def test_dummy():
#     assert True
