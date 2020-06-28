#!/usr/bin/env python
# ----------------------------------------
# brew install libusb
# pip install pyftdi adafruit-blinka
# pip install adafruit-circuitpython-mlx90640
# ----------------------------------------
# FT232H mac: /dev/tty.usbserial-1440
# ----------------------------------------
# camera: 0x33

import time
import board
import busio
import adafruit_mlx90640
import numpy as np
from collections import deque
from colorama import Fore
from pyftdi.i2c import I2cNackError

PRINT_TEMPERATURES = False
PRINT_ASCIIART = True

i2c = busio.I2C(board.SCL, board.SDA, frequency=100000)

mlx = adafruit_mlx90640.MLX90640(i2c)

sn = mlx.serial_number
# sn = sn[0]<<16 + sn[1]<<8 + sn[2]

print("="*50)
print("MLX addr detected on I2C")
print(f" SN: {sn}")
# print([hex(i) for i in mlx.serial_number])
print(" kVdd = %d, vdd25 = %d" % (mlx.kVdd, mlx.vdd25))
print("KvPTAT = %f, KtPTAT = %f, vPTAT25 = %d, alphaPTAT = %f" %
     (mlx.KvPTAT, mlx.KtPTAT, mlx.vPTAT25, mlx.alphaPTAT))
print(" Gain = %d, Tgc = %f, Resolution = %d" % (mlx.gainEE, mlx.tgc, mlx.resolutionEE))
print(" KsTa = %f, ksTo = %s, ct = %s" % (mlx.KsTa, mlx.ksTo, mlx.ct))
print(" cpAlpha:", mlx.cpAlpha, "cpOffset:", mlx.cpOffset)
# print("  alpha: ", mlx.alpha)
print(" alphascale: ", mlx.alphaScale)
# print("  offset: ", mlx.offset)
# print("  kta:", mlx.kta)
print(" ktaScale:", mlx.ktaScale)
# print("  kv:", mlx.kv)
print(" kvScale:", mlx.kvScale)
print(" calibrationModeEE:", mlx.calibrationModeEE)
print(" ilChessC:", mlx.ilChessC)
print("-"*50)

mlx.refresh_rate = adafruit_mlx90640.RefreshRate.REFRESH_2_HZ

frame = [0] * 24*32
# f = deque(32)
d = deque([f"{Fore.BLACK}\u2588"]*32,32)

def scale(t):
    u = "\u2588"
    s = {
        25: f"{Fore.BLACK}{u}",
        27: f"{Fore.BLUE}{u}",
        29: f"{Fore.CYAN}{u}",
        31: f"{Fore.GREEN}{u}",
        33: f"{Fore.YELLOW}{u}",
        35: f"{Fore.RED}{u}",
    }
    for k,v in s.items():
        if t < k:
            return v
    return f"{Fore.RED}{u}"

try:
    while True:
        stamp = time.monotonic()
        try:
            mlx.getFrame(frame)
        except ValueError as e:
            print(e)
            # these happen, no biggie - retry
            continue
        except I2cNackError as e:
            print(e)
            continue

        print("Read a frame in %0.2f s" % (time.monotonic() - stamp))
        for i, t in enumerate(frame):
            if i%32 == 0:
                print("".join(d))

            d.append(scale(t))
        print(Fore.RESET)

except KeyboardInterrupt:
    print(Fore.RESET)
    pass

except Exception as e:
    print(Fore.RESET)
    print(e)
    print("wft")
