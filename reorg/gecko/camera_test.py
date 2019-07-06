#!/usr/bin/env python3
from pygecko.multiprocessing import geckopy
from pygecko import Image
import cv2
from imutils.video import VideoStream
import platform
import time


def publisher(**kwargs):
    geckopy.init_node(**kwargs)
    rate = geckopy.Rate(10)
    pub = geckopy.Publisher()

    if platform.system() == 'Linux':
        args = {
            'src': 0,
            'usePiCamera': True,
            'resolution': (640,480,),
            'framerate': 10
        }
    else:
        args = {
            'src': 0,
            'usePiCamera': False
        }

    cam = VideoStream(**args).start()
    time.sleep(1)

    # camera check
    img = cam.read()
    print(img.shape)
    cv2.imwrite('test.png', img)

    while not geckopy.is_shutdown():
        img = cam.read()
        # img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        msg = Image(img.shape, img.tobytes())
        pub.pub('camera', msg)
        rate.sleep()

    cam.stop()


args = {
    'geckocore': {
        'key': 'nav'
    }
}
publisher()
