#!/usr/bin/env python

from ins_nav.transforms import NED, ENU
import numpy as np
import ins_nav

print(ins_nav.__version__)


n = NED.from_ll(0,45, 1000)
nn = NED.from_ecef(*n.origin)

print(n)
print(nn)

print(n.origin - nn.origin)
print(n.R.dot(nn.R))


a = np.array([100,200,5])
print(n.nav2ecef(a))

print(n == nn)

m = ENU(1,2)
print(n == m)
