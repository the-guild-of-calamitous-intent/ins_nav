#!/usr/bin/env python
import cv2
import numpy as np
from the_collector import BagIt, Pickle

# bagfile = "test-complex.pickle.bag"
bagfile = "test-simple.pickle.bag"
bag = BagIt(Pickle)
data = bag.read(bagfile)

# print(data.keys())
# print(data["imu"][0])

rgb2gray = np.array([0.2989, 0.5870, 0.1140])
bgr2gray = np.array([0.1140, 0.5870, 0.2989])

numc = len(data["camera"])
# numi = len(data["imu"])
start = data["imu"][0][-1]
last = start
for i, (imu, (f,ts),) in enumerate(zip(data["imu"], data["camera"])):
    # f = np.dot(f, rgb2gray).astype(np.uint8)
    # f = np.mean(f, axis=2) # mean across cells
    cv2.imshow(f"camera {f.shape}", f)
    dt = ts-last
    hz = int(1/dt)
    cv2.waitKey(int(1000*dt+1))
    print(f">> Time: {ts-start:.2f} sec   Hz/dt: {hz} Hz/{ts-last:.2f} sec     Camera: {i} of {numc} frames", end="\r")
    last = ts

cv2.destroyAllWindows()
