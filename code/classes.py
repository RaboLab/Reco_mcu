import struct

def float_from_bytes(x):
    return struct.unpack('<f', struct.pack('4B', *x))[0]

class gps():
    year:int
    month:int
    day:int
    
    hour:int
    minute:int
    second:int
    
    latitude:int
    longtitude:int
    weidu:float
    jingdu:float
    speed:float
    
    isOK:int
    def __init__(self):
        pass
    def decode(self, strx:bytes):
        self.year = int.from_bytes(strx[0:2], byteorder='little', signed=False)
        self.month = strx[2]
        self.day = strx[3]
        
        self.hour = strx[4]
        self.minute = strx[5]
        self.second = strx[6]
        
        self.latitude = chr(strx[7])
        self.longtitude = chr(strx[8])
        
        self.weidu = float_from_bytes(strx[9:13])
        self.jingdu = float_from_bytes(strx[13:17])
        self.speed = float_from_bytes(strx[17:21])
        
        self.isOK = strx[21]
        
class imu():
    ax:float
    ay:float
    az:float

    q0:float
    q1:float
    q1:float
    q1:float
    
    roll:float
    pitch:float
    yaw:float
    
    isOK:int
    def __init__(self):
        pass
    def decode(self, strx:bytes):
        self.ax = float_from_bytes(strx[0:4])
        self.ay = float_from_bytes(strx[4:8])
        self.az = float_from_bytes(strx[8:12])
        
        self.q0 = float_from_bytes(strx[16:20])
        self.q1 = float_from_bytes(strx[20:24])
        self.q2 = float_from_bytes(strx[24:28])
        self.q3 = float_from_bytes(strx[28:32])
        
        self.yaw = float_from_bytes(strx[32:36])
        self.pitch = float_from_bytes(strx[36:40])
        self.roll = float_from_bytes(strx[40:44])
        
        self.isOK = strx[44]
        
        
        
        
        
        
    