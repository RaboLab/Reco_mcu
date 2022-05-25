import serial
from classes import float_from_bytes
from classes import gps
from classes import imu


ser = serial.Serial('/dev/ttyAMA0', 115200, timeout=10)
print('ser opened')
gpsx = gps()
imux = imu()
while True:
    strx = ser.read_until(b'\x0A\x0A')
    if(len(strx) == 24):
        gpsx.decode(strx)
        
        #print(f'Year:{gpsx.year}\tMonth:{gpsx.month}\t\tDay:{gpsx.day}')
        #print(f'Hour:{gpsx.hour}\t\tMinute:{gpsx.minute}\tSecond:{gpsx.second}')
        #print(f'NS:{gpsx.latitude}\t\tLatitude:{gpsx.weidu}\tEW:{gpsx.longtitude}\tLongtitude:{gpsx.jingdu}')
        #print(f'isOK:{gpsx.isOK}\t\tSpeed:{gpsx.speed}')
        
    elif(len(strx) == 47):
        imux.decode(strx)
        
        print(f'Yaw:{imux.yaw:<3.8f}\t\tPicth:{imux.pitch:<3.8f}\t\tRoll:{imux.roll:<3.8f}')
        print(f'ax:{imux.ax:<3.8f}\t\t\tay:{imux.ay:<3.8f}\t\t\taz:{imux.az:<3.8f}')
        print(f'q0:{imux.q0:<3.8f}\t\t\tq1:{imux.q1:<3.8f}\t\t\tq2:{imux.q2:<3.8f}\tq3:{imux.q3:<3.8f}')
        print(f'isOK:{imux.isOK}')
                                                                                                            