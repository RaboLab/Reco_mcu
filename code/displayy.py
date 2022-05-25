import os
import sys
import matplotlib.pyplot as plt
import pickle
import numpy as np
import imu_anti_gravity

with open('IMU2', 'rb') as f:
    data = pickle.load(f)
count = len(data)
print(count)
imu = imu_anti_gravity.anti_gravity(data, count)

fig,a =  plt.subplots(3, 2, figsize=(15,12))

x_axis = np.arange(0, count)

a[0][0].set_title('Accels X')
a[0][0].plot(x_axis, imu.ax, label='ax')
a[0][0].legend()
a[1][0].set_title('Accels Y')
a[1][0].plot(x_axis, imu.ay, label='ay')
a[1][0].legend()
a[2][0].set_title('Accels Z')
a[2][0].plot(x_axis, imu.az, label='az')
a[2][0].legend()

a[0][1].set_title('Angle yaw')
a[0][1].plot(x_axis, imu.yaw, label='yaw')
a[0][1].legend()
a[1][1].set_title('Angle ptich')
a[1][1].plot(x_axis, imu.pitch, label='pitch')
a[1][1].legend()
a[2][1].set_title('Angle roll')
a[2][1].plot(x_axis, imu.roll, label='roll')
a[2][1].legend()

plt.show()
#a[0].set_ylim(-3, 0.5)
#a[1].set_ylim(-16, 16)