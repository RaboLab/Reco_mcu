import time
import threading
import subprocess
import os

com = []
com.append("ffmpeg")
com.append("-y")
com.append("-f")
com.append("v4l2")
com.append("-thread_queue_size")
com.append("512")
com.append("-i")
com.append("/dev/video2")
com.append("-f")
com.append("alsa")
com.append("-thread_queue_size")
com.append("512")
com.append("-i")
com.append("hw:4,0")
com.append("-codec")
com.append("copy")
com.append("video.mkv")

class subThread(threading.Thread):
    def __init__(self, time, audio_dev, folder):
        super().__init__()
        
        self.logtime = time
        self.path = folder + '/'
        com[13] = "hw:" + str(audio_dev) + ",0"
    def run(self):
        os.chdir(self.path)
        try:
            subprocess.run(com, shell=False, timeout=self.logtime)
        except subprocess.TimeoutExpired as te:
            print("end") 