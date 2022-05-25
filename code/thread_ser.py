import time
import serial
import threading
import os
import pickle
import random

from classes import gps
from classes import imu


def defuse(strx):
    if len(strx) == 47:  # IMU
        imux = imu()
        imux.decode(strx)
        return imux, 1
    elif len(strx) == 24:
        gpsx = gps()
        gpsx.decode(strx)
        return gpsx, 2
    else:
        return [], 0


class subThread(threading.Thread):
    ser = serial.Serial()
    path = 'empty/'

    __flag = threading.Event()
    __run = threading.Event()

    IMU = []
    GPS = []

    def __init__(self, serialx, filefolder):
        super().__init__()

        self.__flag.set()
        self.__run.set()  # xxx.wait()  return immediately when True

        if not serialx:
            raise SubSerhasnoserialportError
        if not filefolder:
            raise HasNoFilefolderError

        self.ser = serialx
        self.path = filefolder + '/'

    def run(self):
        while self.__run.isSet():
            # while True
            strx = self.ser.read_until(b'\x0A\x0A')
            data, where = defuse(strx)
            if where == 1:
                self.IMU.append(data)
                with open(f"{self.path}IMU", "wb") as f:
                    pickle.dump(self.IMU, f)
            elif where == 2:
                self.GPS.append(data)
                with open(f"{self.path}GPS", "wb") as f:
                    pickle.dump(self.GPS, f)
            time.sleep(0.1)
        # clean up

    def stop(self):
        self.__run.clear()
