#!/usr/bin/env python

import numpy as np
import ins_nav
import time
from nxp_imu import IMU
from slurm.rate import Rate
from slurm.simple_process import SimpleProcess
from the_collector import BagIt, Pickle
from picamera import PiCamera
import picamera.array
import imageio
from colorama import Fore
import imageio
import picamera
import io
from threading import Thread


class ThreadedCamera:
    """
    c = ThreadedCamera((640,480))
    c.start()       # starts internal loop
    frame = c.get() # numpy array
    c.stop()        # stops internal loop
    c.join()        # gathers back up the thread
    """
    def __init__(self, resolution=None, fps=30):
        if resolution is None:
            res = (1024, 768) # col, row
        else:
            res = resolution

        self.camera = PiCamera()
        self.camera.framerate = fps
        self.camera.resolution = res
        self.output = picamera.array.PiRGBArray(self.camera, size=res)
        self.stream = self.camera.capture_continuous(
            self.output,
            format="bgr",
            use_video_port=True)
        # time.sleep(2) # camera warm-up
        self.frame = None
        self.run = False
        # self.bytesio = io.BytesIO()

    def __del__(self):
        self.run = False
        self.camera.close()
        self.output.close()

    def start(self):
        """Starts the internal loop in a thread"""
        self.run = True
        self.ps = Thread(target=self.thread_func, args=())
        self.ps.daemon = True
        self.ps.start()
        return self

    def stop(self):
        """Stops the camera"""
        self.run = False
        time.sleep(0.5)

    def get(self):
        """Returns image frame"""
        return self.frame

    def thread_func(self):
        """Internal function, do not call"""
        # print(f">> self.run: {self.run}")
        bytesio = io.BytesIO()
        # im = imageio.Image()

        for f in self.stream:
            self.frame = f.array.copy()
            print(f">> thread: {self.frame.shape}")
            # ff = imageio.imwrite(bytesio, self.frame, format='png')
            self.output.truncate(0)
            if not self.run:
                self.stream.close()
                self.output.close()
                self.camera.close()
                return

    def join(self, timeout=1.0):
        """
        Attempts to join() the process with the given timeout. If that fails, it calls
        terminate().
        timeout: how long to wait for join() in seconds.
        """
        # print('>> Stopping Simple Process {}[{}] ...'.format(self.ps.name, self.ps.pid))
        # print(f'>> {Fore.RED}Stopping{Fore.RESET}: {self.ps.name}[{self.ps.pid}] ...')
        if self.ps:
            self.ps.join(timeout)
            if self.ps.is_alive():
                self.ps.terminate()
        self.ps = None


bag = BagIt(Pickle)
imu = IMU(gs=2, dps=2000, verbose=True)
rate = Rate(1)


camera = ThreadedCamera()
camera.start()

try:
    while True:
        a, m, g = imu.get()
        print('| {:>5.2f} {:>5.2f} {:>5.2f} | {:>6.1f} {:>6.1f} {:>6.1f} | {:>6.1f} {:>6.1f} {:>6.1f} |'.format(
            a[0], a[1], a[2],
            m[0], m[1], m[2],
            g[0], g[1], g[2])
        )

        frame = camera.get()
        if frame is None:
            print(f"{Fore.RED}*** Camera Fail ***{Fore.RESET}")
        else:
            print(f">> {frame.shape}")

        rate.sleep()

except KeyboardInterrupt:
    camera.stop()
    camera.join()
    print(">> bye ...")
