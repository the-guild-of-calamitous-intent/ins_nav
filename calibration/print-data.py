#!/usr/bin/env python

from __future__ import division, print_function
from the_collector import BagReader


bag = BagReader()
data = bag.read('data-still.bag')

for a in data['accel']:
    print(a)
