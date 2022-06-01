import numpy as np
import cv2


class Compressor:
    """
    Compressor allow you to serialize and compress an image using either JPEG
    or PNG compression.
    """

    _format = ".png"

    @property
    def format(self):
        return self._format

    @format.setter
    def format(self, fmt):
        """
        Set format to either .jpg jpg .png png
        """
        if fmt not in [".jpg", "jpg", ".png", "png"]:
            raise ValueError(f"Invalid format: {fmt}")
        if fmt.find(".") != 0:
            fmt = "." + fmt
        self._format = fmt

    def compress(self, img):
        ok, cb = cv2.imencode(self._format, img)
        if ok:
            cb = cb.tobytes()
        else:
            cb = None
        return cb

    def uncompress(self, img_bytes, shape):
        img = np.frombuffer(img_bytes, dtype=np.uint8)

        if len(shape) == 3:
            img = cv2.imdecode(img, cv2.IMREAD_COLOR)
        else:
            img = cv2.imdecode(img, cv2.IMREAD_GRAYSCALE)

        img = img.reshape(shape)
        return img
    
########################################################

from IPython.display import HTML, display

from IPython.utils import openpy
from pathlib import Path
import inspect

from pygments import highlight
from pygments.lexers import PythonLexer
from pygments.formatters import HtmlFormatter

def getCode(url=None, imp=None, file=None):
    if imp is not None:
        py = inspect.getsource(imp)
    elif url is not None:
        py = openpy.read_py_url(url)
    elif file is not None:
        py = openpy.read_py_file(str(file))
    display(HTML(highlight(py, PythonLexer(), HtmlFormatter())))
    
########################################################

import pickle

def loadPickle(filename):
    with open(filename, 'rb') as fd:
        d = pickle.load(fd)
    return d

def getData(filename):
    imu = {
        "accel": [],
        "gyro": [],
        "mag": [],
        "pres": [],
        "temp": [],
        "stamp": []
    }
    
    bno = {
        "euler": {
            "roll": [],
            "pitch": [],
            "yaw": [],
            "time": []
        },
        "q": {
            "w": [],
            "x": [],
            "y": [],
            "z": [],
            "time": []
        }
    }
    
    imgs = []
    comp = Compressor()
    
    stamp = []
    
    data = loadPickle(filename)
    tstart = data[0][-1] # last value is always time
    
    for d in data:
        try:
            a,g,m,p,t,q,e,dt = d
            imu["accel"].append(a)
            imu["gyro"].append(g)
            imu["mag"].append(m)
            imu["pres"].append(p)
            imu["temp"].append(t)
            imu["stamp"].append(dt - tstart)

            bno["q"]["w"].append(q[0])
            bno["q"]["x"].append(q[1])
            bno["q"]["y"].append(q[2])
            bno["q"]["z"].append(q[3])
            bno["q"]["time"].append(dt - tstart)
            bno["euler"]["roll"].append(e[0])
            bno["euler"]["pitch"].append(e[1])
            bno["euler"]["yaw"].append(e[2])
            bno["euler"]["time"].append(dt - tstart)

            stamp.append(dt - tstart)
        except ValueError:
            a,g,m,p,t,q,e,(b, shape), dt = d
            imu["accel"].append(a)
            imu["gyro"].append(g)
            imu["mag"].append(m)
            imu["pres"].append(p)
            imu["temp"].append(t)
            imu["stamp"].append(dt - tstart)

            bno["q"]["w"].append(q[0])
            bno["q"]["x"].append(q[1])
            bno["q"]["y"].append(q[2])
            bno["q"]["z"].append(q[3])
            bno["q"]["time"].append(dt - tstart)
            bno["euler"]["roll"].append(e[0])
            bno["euler"]["pitch"].append(e[1])
            bno["euler"]["yaw"].append(e[2])
            bno["euler"]["time"].append(dt - tstart)
            
            im = comp.uncompress(b, shape)
            imgs.append(im)

            stamp.append(dt - tstart)
            
    return imgs, imu, bno, stamp