import matplotlib.pyplot as plt
import numpy as np
import txt_to_imu
# input : imu 

# imu, count = txt_to_imu.txt_to_imu('./data/every_pose.txt')
# imu, count = txt_to_imu.txt_to_imu('data/total_test_xbw.txt')
imu, count = txt_to_imu.txt_to_imu('biaodin.txt')

fig,a =  plt.subplots(3, 1)

x_axis = np.arange(0, count)

a[0].set_title('Accel Compare  X_axis')
a[0].plot(x_axis, imu[0].acc_x, label='HELMET')
a[0].plot(x_axis, imu[1].acc_x, label='CAMERA')
a[0].plot(x_axis, imu[2].acc_x, label='FACE')
a[0].legend()

a[1].set_title('Accel Compare  Y_axis')
a[1].plot(x_axis, imu[0].acc_y, label='HELMET')
a[1].plot(x_axis, imu[1].acc_y, label='CAMERA')
a[1].plot(x_axis, imu[2].acc_y, label='FACE')
a[1].legend()

a[2].set_title('Accel Compare  Z_axis')
a[2].plot(x_axis, imu[0].acc_z, label='HELMET')
a[2].plot(x_axis, imu[1].acc_z, label='CAMERA')
a[2].plot(x_axis, imu[2].acc_z, label='FACE')
a[2].legend()

a[0].set_ylim(-16, 16)
a[1].set_ylim(-16, 16)
a[2].set_ylim(-16, 16)
plt.figure(1)

fig,a =  plt.subplots(3, 1)
a[0].set_title('Angel Compare ROLL')
a[0].plot(x_axis, imu[0].angle_x, label='HELMET')
a[0].plot(x_axis, imu[1].angle_x, label='CAMERA')
a[0].plot(x_axis, imu[2].angle_x, label='FACE')
a[0].legend()

a[1].set_title('Angel Compare PITCH')
a[1].plot(x_axis, imu[0].angle_y, label='HELMET')
a[1].plot(x_axis, imu[1].angle_y, label='CAMERA')
a[1].plot(x_axis, imu[2].angle_y, label='FACE')
a[1].legend()

a[2].set_title('Angel Compare YAW')
a[2].plot(x_axis, imu[0].angle_z, label='HELMET')
a[2].plot(x_axis, imu[1].angle_z, label='CAMERA')
a[2].plot(x_axis, imu[2].angle_z, label='FACE')
a[2].legend()

plt.show()