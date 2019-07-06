"""
https://www.mathworks.com/help/aerotbx/ug/ecef2lla.html
ecef2lla([4510731 4510731 0]) => 0   45.0000  999.9564
ecef2lla([0 4507609 4498719], 'WGS84') => 45.1358   90.0000  999.8659

https://www.mathworks.com/help/aerotbx/ug/lla2ecef.html
lla2ecef([0 45 1000]) => 1.0e+006 * 4.5107    4.5107         0
lla2ecef([45 90 2000], 'WGS84') => 1e6 * 0.0000    4.5190    4.4888

Can also double check with https://www.ngs.noaa.gov/NCAT/
"""

from transforms import ecef2llh, llh2ecef

def test_transforms():
	assert ecef2lla(4510731, 4510731, 0) == (0,   45.0000,  999.9564,)
    assert lla2ecef(0, 45, 1000) == (4.5107*1e6,    4.5107*1e6, 0)

def test_dummy():
	assert True
