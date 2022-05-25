import datetime
import time
import os
import sys
import threading
import shutil

import serial
import RPi.GPIO as GPIO

import thread_ser as subser
import thread_reco as sublog

import file_move

FOLDER_MAX = 100
log_time = 20 #60sec

def set_work_folder() -> str:
    return '/home/pi/Desktop/upup/videos/cycle/'

def get_folder_count(dir: str) -> int:
    file_cnt = 0
    for f in os.listdir(dir):
        file_cnt += 1
    return file_cnt

def get_audio_dev() -> int:
    strx = os.popen('arecord -l').read()
    return int(strx[strx.find('UACDemoV10') - 3])

def set_reco_folder(work_folder: str) -> str:
    curr_time = datetime.datetime.now()
    return work_folder + curr_time.strftime("%Y-%m-%d-%H-%M-%S"), curr_time.strftime("%Y-%m-%d-%H-%M-%S")

def delete_when_reach_max(work_dir: str, maxv:int) -> None:
    folder_list = os.listdir(work_dir)
    folder_list.sort()
    
    while len(folder_list) > maxv:
        del_folder = work_dir + folder_list[0] + '/'

        for f in os.listdir(del_folder):
            fil = del_folder + f
            if os.path.isfile(fil):
                os.remove(fil)
            else:
                for f in os.listdir(fil):
                    os.remove(fil + '/' + f)
                os.rmdir(fil)
        os.rmdir(del_folder)
        folder_list = os.listdir(work_dir)
        folder_list.sort()
def copy_notebook(folder_name: str):
    src_path = '/home/pi/Desktop/upup/code/display.ipynb'
    shutil.copyfile(src_path, folder_name + '/display.ipynb')
if __name__ == '__main__':
    #power up procegure
    time.sleep(10)
    #GPIO Setting
    GPIO.setmode(GPIO.BCM)
    
    OnlinePin = 18   # 1 = Record, 0 = poweroff
    GPIO.setup(OnlinePin, GPIO.IN, pull_up_down = GPIO.PUD_UP)
    SavePin = 23   # 1 = isSave, 0 = Dont save
    GPIO.setup(SavePin, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
    LookPin = 24   # 1 = Record, 0 = poweroff
    GPIO.setup(LookPin, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
    OutPin = 25   # 1 = Record, 0 = poweroff
    GPIO.setup(OutPin, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
    
    OnlineValue = GPIO.input(OnlinePin)
    #Serial Setting
    #Serial Setting
    ser = serial.Serial('/dev/serial0', 115200, timeout=10)  #if timeout, consider IMU\GPS offline
    #time setting
    update_flag = 0
    while not update_flag:
        strx = ser.read_until(b'\x0A\x0A')
        data, where = subser.defuse(strx)
        if where == 2:
            current_time="%u-%02u-%02u %02u:%02u:%02u"%(data.year, data.month, data.day, data.hour, data.minute, data.second)
            os.system('sudo date -s "'+current_time+'"')
            update_flag = 1
            
    #arecord setting
    audio_dev = get_audio_dev() 
    #Folder Setting
    work_folder = set_work_folder()   # videos/cycle/
    
    while True:
        OnlineValue = GPIO.input(OnlinePin)
        LookValue = GPIO.input(LookPin)
        if OnlineValue:
            if not LookValue:
                print("LOG")
                #Folder Management
                delete_when_reach_max(work_folder, FOLDER_MAX)
                
                folder_name, curr_time = set_reco_folder(work_folder)
                #xxx/cycle/2022-3-4-12-23-59  2022-3-4-12-23-59 etc
                #create folder
                os.makedirs(folder_name, exist_ok = True)   # make xxx/xxx/2022-3-4-12-23-59/
                
                #Copy Jupyter Notebook
                copy_notebook(folder_name)
                #Create SubThread
                tser = subser.subThread(ser, folder_name)  # serial, folder
                tlog = sublog.subThread(log_time, audio_dev, folder_name)
                #Start Record
                tlog.start()
                tser.start()

                tlog.join()  #Block until log is over
                tser.stop()  #stop tser
                tser.join()
                #Save or not
                SaveValue = GPIO.input(SavePin)
                print(f'Save Pin read: {SaveValue}')
                if(SaveValue):
                    ser.write(b'\t\t\t')
                    file_move.copy_dir(folder_name,curr_time)
                #next loop:
            else:
                print("LOOKVIDEO")
                time.sleep(5)
                ser.write(b'\t\t\t')
        else:         #ONLINE 0
            if not LookValue:   #LOOK 0
                print("POWEROFF")
                ser.write(b'\r\r\r')
                time.sleep(1)
                os.system('poweroff')
            else:
                print("EXIT")
                ser.write(b'\n\n\n')
                time.sleep(1)
                sys.exit()
                #os.system('poweroff')
