#!/usr/bin/env python
# 6 July 2019 Kevin Walchko, MIT License
# Just prints data from the bag file

from the_collector import BagIt, Pickle
from collections import namedtuple

Data = namedtuple('Data', 'data timestamp')

bag = BagIt(Pickle)
data = bag.read('data.pickle.bag')

start = data['accel'][0].timestamp

def p(header, v, units):
    print("{:>6}: {:6.1f} {:6.1f} {:6.1f} {}".format(
        header,
        v.data[0],
        v.data[1],
        v.data[2],
        units
    ))

for a, m, g in zip(data['accel'], data['mag'], data['gyro']):
    print("[{:.3f} sec]--------------------------------".format(a.timestamp-start))
    p("Accel", a, "[g's]")
    p("Mag", m, "[uT]")
    p("Gyro", g, "[rads/sec]")
