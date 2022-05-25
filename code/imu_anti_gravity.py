from scipy.spatial.transform import Rotation as R

class imu_t:
    
    def __init__(self):
        self.yaw = []
        self.pitch = []
        self.roll = []

        self.q0 = []
        self.q1 = []
        self.q2 = []
        self.q3 = []

        self.ax = []
        self.ay = []
        self.az = []
        pass
    
def anti_gravity(imu: list, count) -> list:
    imut = imu_t();
    for x in range(count):
        q0 = imu[x].q0   # w
        q1 = imu[x].q1   # x 
        q2 = imu[x].q2   # y
        q3 = imu[x].q3   # z
        r = R.from_quat([q1, q2, q3, q0]) # x y z w
        r_inv = r.inv()
        
        [rotated_x, rotated_y, rotated_z] = r_inv.apply([0, 0, 1])
        imu[x].ax -= 0
        imu[x].ay -= 0
        imu[x].az -= 0
        # imu[x].ax -= rotated_x
        # imu[x].ay -= rotated_y
        # imu[x].az -= rotated_z
        # temp = imu[x].ax
        # imu[x].ax = imu[x].ay
        # imu[x].ay = -temp
    for x in range(count):
        imut.yaw.append(imu[x].yaw)
        imut.pitch.append(imu[x].pitch)
        imut.roll.append(imu[x].roll)
        
        imut.q0.append(imu[x].q0)
        imut.q1.append(imu[x].q1)
        imut.q2.append(imu[x].q2)
        imut.q3.append(imu[x].q3)
        
        imut.ax.append(imu[x].ax)
        imut.ay.append(imu[x].ay)
        imut.az.append(imu[x].az)
    return imut
        