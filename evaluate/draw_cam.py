#!/usr/bin/env python
# coding=utf-8

import numpy as np
import matplotlib.pyplot as plt
# import mpl_toolkits.mplot3d
from mpl_toolkits.mplot3d import Axes3D

f = open("./lab/FrameTrajectory_TUM_Format.txt")
x = []
y = []
z = []
for line in f:
    data = line.split()
    x.append( float(data[1] ) )
    y.append( float(data[2] ) )
    z.append(float(0.0))
# ax = plt.subplot( 111)
fig = plt.figure()
ax = Axes3D(fig)
ax.plot(x, y, z)
# ax.scatter(x, y, z)
plt.show()
