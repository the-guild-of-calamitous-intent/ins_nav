
from picamera import PiCamera
import picamera.array
import imageio
from colorama import Fore
import picamera
import io
from threading import Thread, Lock
import time
from slurm.rate import Rate
import numpy as np

rgb2g = np.array([0.2989, 0.5870, 0.1140])
bgr2g = np.array([0.1140, 0.5870, 0.2989])

rgb2gray = lambda im: np.dot(im, rgb2g).astype(np.uint8)
bgr2gray = lambda im: np.dot(im, bgr2g).astype(np.uint8)

class ThreadedCamera:
    """
    https://www.raspberrypi.org/documentation/hardware/camera/
    Raspberry Pi v2:
        resolution: 3280 x 2464 pixels
        sensor area: 3.68 mm x 2.76 mm
        pixel size: 1.12 um x 1.12 um
        video modes:1080p30, 720p60 and 640x480p60/90
        optical size: 1/4"
        driver: V4L2 driver

    c = ThreadedCamera((640,480))
    c.start()       # starts internal loop
    frame = c.get() # numpy array
    c.stop()        # stops internal loop
    c.join()        # gathers back up the thread
    """
    def __init__(self, resolution=None, fps=30, fmt='rgb'):
        if resolution is None:
            res = (1024, 768) # col, row
        else:
            res = resolution

        self.fmt = fmt
        if fmt == "rgb":
            self.c2g = np.array([0.2989, 0.5870, 0.1140])
        elif fmt == "bgr":
            self.c2g = np.array([0.1140, 0.5870, 0.2989])
        elif fmt == "gray":
            fmt = "rgb"
            self.fmt = "gray"
            self.c2g = np.array([0.2989, 0.5870, 0.1140])
        else:
            raise Exception(f"Unknown color format: {fmt}")

        self.camera = PiCamera()
        self.camera.framerate = fps
        self.camera.resolution = res
        # self.camera.awb_mode = "off"  # black images?
        # self.exposure_mode = "fixedfps" # value?
        self.output = picamera.array.PiRGBArray(self.camera, size=res)
        self.stream = self.camera.capture_continuous(
            self.output,
            format=fmt, # bgr for opencv
            # format="bgr", # bgr for opencv
            # format="png",
            use_video_port=True)
        # time.sleep(2) # camera warm-up
        self.frame = None
        self.run = False
        # self.bytesio = io.BytesIO()
        # self.new_frame = False
        self.lock = Lock()
        self.start()

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
        """Stops the camera and the thread that is constantly grabbing images"""
        self.run = False
        time.sleep(0.5)

    # def rgb2gray(self, im):
    #     return np.dot(im, rgb2gray).astype(np.uint8)

    def color2gray(self, im):
        return np.dot(im, self.c2g).astype(np.uint8)

    def read(self):
        """Returns image frame"""
        if self.frame is None:
            return None

        if self.fmt == "gray":
            return np.dot(self.frame, self.c2g).astype(np.uint8)
        else:
            return frame

    # def get_gray(self):
    #     if self.frame is None:
    #         return None
    #
    #     self.lock.acquire()
    #     f = np.dot(self.frame, rgb2gray).astype(np.uint8)
    #     self.lock.release()
    #     return f

    def thread_func(self):
        """Internal function, do not call"""
        # print(f">> self.run: {self.run}")
        # bytesio = io.BytesIO()
        # im = imageio.Image()
        # rate = Rate(30)

        for f in self.stream:
            self.lock.acquire()
            # self.frame = f.array.copy()
            self.frame = f.array
            self.lock.release()
            # print(f"++ thread: {self.frame.shape}")
            # ff = imageio.imwrite(bytesio, self.frame, format='png')
            self.output.truncate(0)
            # self.new_frame = True
            if not self.run:
                self.stream.close()
                self.output.close()
                self.camera.close()
                return
            # rate.sleep()

    def write(self, filename):
        self.lock.acquire()
        imageio.imwrite(filename, self.frame)
        self.lock.release()

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
            # if self.ps.is_alive():
            #     self.ps.terminate()
        self.ps = None


## PiCamera

# class SplitFrames(object):
#     def __init__(self):
#         self.frame_num = 0
#         self.output = None
#
#     def write(self, buf):
#         if buf.startswith(b'\xff\xd8'):
#             # Start of new frame; close the old one (if any) and
#             # open a new output
#             if self.output:
#                 self.output.close()
#             self.frame_num += 1
#             self.output = io.open('image%02d.jpg' % self.frame_num, 'wb')
#         self.output.write(buf)
#
# with picamera.PiCamera(resolution='720p', framerate=30) as camera:
#     camera.start_preview()
#     # Give the camera some warm-up time
#     time.sleep(2)
#     output = SplitFrames()
#     start = time.time()
#     camera.start_recording(output, format='mjpeg')
#     camera.wait_recording(2)
#     camera.stop_recording()
#     finish = time.time()
# print('Captured %d frames at %.2ffps' % (
#     output.frame_num,
#     output.frame_num / (finish - start)))
