#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
import multiprocessing as mp
import time
import signal

# from pygecko.transport import Pub, Sub
# from pygecko.transport.zmqclass import SubNB
# from pygecko.transport import zmqTCP
from pygecko.multiprocessing import geckopy
from math import sin, cos, pi, sqrt

from nxp_imu import IMU
from pydar import LDS01
from the_collector import BagWriter
import platform


if system == 'Darwin':
    port = "/dev/tty.SLAB_USBtoUART"
elif system == 'Linux':
    # also valid
    # /dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0
    port = "/dev/ttyUSB0"
else:
    raise Exception("unsupported OS")
