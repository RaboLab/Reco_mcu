# @brief: After capture, rotate every IMU from this own coordinates to world coordinates

#         | Z
#         |  /X
#         | /
# Y ______|/

# keywords: class-IMU

import os

import numpy as np
import class_imu
import txt_to_imu
import matplotlib.pyplot as plt

# /////////  Micro Define /////////
imuFaceID = 2
imuHelmetID = 0
imuCameraID = 1
# /////////  Micro Define /////////

imuFace   = class_imu.jy62(imuFaceID)
imuHelmet = class_imu.jy62(imuHelmetID)
imuCamera = class_imu.jy62(imuCameraID)

def trans_method(imu: list) -> tuple:      # translate this into rotation matrix later
    global imuFace
    global imuHelmet
    global imuCamera
    # FACE   X=-Y  Y=X   (WORLD AXIS = IMU AXIS)
    imuFace.acc_x = (-np.array(imu[imuFaceID].acc_y)).tolist()
    imuFace.wx = (-np.array(imu[imuFaceID].wy)).tolist()
    imuFace.angle_x = (-np.array(imu[imuFaceID].angle_y)).tolist()
    imuFace.acc_y = (np.array(imu[imuFaceID].acc_x)).tolist()
    imuFace.wy = (np.array(imu[imuFaceID].wx)).tolist()
    imuFace.angle_y = (np.array(imu[imuFaceID].angle_x)).tolist()
    imuFace.acc_z = np.array(imu[imuFaceID].acc_z).tolist()
    imuFace.wz = np.array(imu[imuFaceID].wz).tolist()
    imuFace.angle_z = np.array(imu[imuFaceID].angle_z).tolist()
    imuFace.total_z = np.array(imu[imuFaceID].total_z).tolist()
    # HELMET  X  = -X  Y = -Y
    imuHelmet.acc_x = (-np.array(imu[imuHelmetID].acc_x)).tolist()
    imuHelmet.wx = (-np.array(imu[imuHelmetID].wx)).tolist()
    imuHelmet.angle_x = (-np.array(imu[imuHelmetID].angle_x)).tolist()
    imuHelmet.acc_y = (-np.array(imu[imuHelmetID].acc_y)).tolist()
    imuHelmet.wy = (-np.array(imu[imuHelmetID].wy)).tolist()
    imuHelmet.angle_y = (-np.array(imu[imuHelmetID].angle_y)).tolist()
    imuHelmet.acc_z = np.array(imu[imuHelmetID].acc_z).tolist()
    imuHelmet.wz = np.array(imu[imuHelmetID].wz).tolist()
    imuHelmet.angle_z = np.array(imu[imuHelmetID].angle_z).tolist()
    imuHelmet.total_z = np.array(imu[imuHelmetID].total_z).tolist()
    # CAMERA  X=Y Y=-X
    imuCamera.acc_x = imu[imuCameraID].acc_y
    imuCamera.wx = imu[imuCameraID].wy
    imuCamera.angle_x = imu[imuCameraID].angle_y
    imuCamera.acc_y = (-np.array(imu[imuCameraID].acc_x)).tolist()
    imuCamera.wy = (-np.array(imu[imuCameraID].wx)).tolist()
    imuCamera.angle_y = (-np.array(imu[imuCameraID].angle_x)).tolist()
    imuCamera.acc_z = np.array(imu[imuCameraID].acc_z).tolist()
    imuCamera.wz = np.array(imu[imuCameraID].wz).tolist()
    imuCamera.angle_z = np.array(imu[imuCameraID].angle_z).tolist()
    imuCamera.total_z = np.array(imu[imuCameraID].total_z).tolist() 
    return imuFace, imuHelmet, imuCamera


# imu, count = txt_to_imu.txt_to_imu('angle_test.txt')
# imuFace, imuHelmet, imuCamera = trans_method(imu)

# fig, a = plt.subplots(3, 1)
# x_axis = np.arange(0, count)
# x_axis = x_axis / 200
# i = 0
# a[i].set_title('Angle ROLL')
# a[i].plot(x_axis, imuFace.angle_x, label='imuFace')
# a[i].plot(x_axis, imuHelmet.angle_x, label='imuHelmet')
# a[i].plot(x_axis, imuCamera.angle_x, label='imuCamera')
# a[i].legend()

# i = 1
# a[i].set_title('Angle PITCH')
# a[i].plot(x_axis, imuFace.angle_y, label='imuFace')
# a[i].plot(x_axis, imuHelmet.angle_y, label='imuHelmet')
# a[i].plot(x_axis, imuCamera.angle_y, label='imuCamera')
# a[i].legend()

# i = 2
# a[i].set_title('Angle YAW')
# a[i].plot(x_axis, imuFace.angle_z, label='imuFace')
# a[i].plot(x_axis, imuHelmet.angle_z, label='imuHelmet')
# a[i].plot(x_axis, imuCamera.angle_z, label='imuCamera')
# a[i].legend()
# plt.show()
