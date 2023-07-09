#!/usr/bin/env python
# pip install opencv-python-headless
import numpy as np
# import cv2
import picamera
import time
import imageio
# from imutils.video import VideoStream

with picamera.PiCamera() as camera:
    camera.resolution = (320, 240)
    camera.framerate = 24
    time.sleep(2)
    output = np.empty((240, 320, 3), dtype=np.uint8)
    camera.capture(output, 'rgb')
    
imageio.imwrite('test.png', output)
