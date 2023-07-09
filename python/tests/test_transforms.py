"""
https://www.mathworks.com/help/aerotbx/ug/ecef2lla.html
ecef2lla([4510731 4510731 0]) => 0   45.0000  999.9564
ecef2lla([0 4507609 4498719], 'WGS84') => 45.1358   90.0000  999.8659

https://www.mathworks.com/help/aerotbx/ug/lla2ecef.html
lla2ecef([0 45 1000]) => 1.0e+006 * 4.5107    4.5107         0
lla2ecef([45 90 2000], 'WGS84') => 1e6 * 0.0000    4.5190    4.4888

Can also double check with https://www.ngs.noaa.gov/NCAT/
"""

from ins_nav import *
import pytest
import numpy as np

data = [
    # ecef [m,m,m]    lat,lon,alt [deg,deg,m]
    ((5.057590377e6, 2.694861463e6, -2.794229000e6), (-26.145055, 28.050255, 1660.86)), # Pretoria S. Africa
    ((-4.646678571e6, 2.549341033e6, -3.536478881e6), (-33.89115, 151.249186, 86.26)),  # Sydney Australia
    ((4510731, 4510731, 0), (0, 45, 999.9564)),      # matlab
    ((0.0000, 4.5190e6, 4.4888e6), (45, 90, 2000)),  # matlab
]

def test_local_frames():
    ned = [500,600,-400]
    enu = [600,500,400]

    f = NavigationFrame((0,0,0))

    assert np.allclose(f.ned2enu(ned), enu)
    assert np.allclose(f.enu2ned(enu), ned)

def test_ecef():
    wgs = WGS84()

    for ecef, lla in data:
        assert np.allclose( wgs.llh2ecef(lla), ecef )

        # the altitude doesn't always come close ... so only look at lat, lon values
        assert np.allclose( wgs.ecef2llh(ecef)[:2], lla[:2] ), f"{wgs.ecef2llh(ecef)}, {lla}"
