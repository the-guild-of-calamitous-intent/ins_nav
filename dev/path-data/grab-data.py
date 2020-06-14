#!/usr/bin/env python

import numpy as np
import time
from nxp_imu import IMU
from slurm.rate import Rate
from the_collector import BagIt, Pickle
from colorama import Fore
from threaded_camera import ThreadedCamera

thumb = False
bag = BagIt(Pickle)
imu = IMU(gs=2, dps=2000, verbose=True)
rate = Rate(20)

# res = (3008,480)
# res = (1024,720)
# res = (640,480)
res = (320,240)
camera = ThreadedCamera(res, fmt="gray")

start = time.time()
last = start
try:
    while True:
        a, m, g = imu.get()
        ts = time.time()
        dt = ts - last
        hz = int(1/dt)
        last = ts
        print('{} Hz/{:.2f} s  | {:>5.2f} {:>5.2f} {:>5.2f} | {:>6.1f} {:>6.1f} {:>6.1f} | {:>6.1f} {:>6.1f} {:>6.1f} |'.format(
            hz,dt,
            a[0], a[1], a[2],
            m[0], m[1], m[2],
            g[0], g[1], g[2]),
            end = "\r"
        )
        bag.push("imu", (a,g,m,time.time(),))

        frame = camera.read()

        if frame is None:
            print(f"{Fore.RED}*** Camera Fail ***{Fore.RESET}")
        else:
            bag.push("camera", (frame,time.time(),))

        rate.sleep()

except KeyboardInterrupt:
    camera.stop()
    camera.join()
    print(">> bye ...")
    bag.write("test", timestamp=False)
