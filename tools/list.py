#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
king@2018.5.12
# wirte a test list.txt file
"""

import os
from scipy.io import loadmat as sio
import numpy as np
import PIL.Image as Image

os.chdir('/home/king/Documents/image/scene_09/depth')
path=os.getcwd()    # /home/king/Documents/image/scene_09/rgb
files=os.listdir(path)  # 00118-color.png
# print "files: ", files
files.sort()  # sort in order
 
with open('./depth.txt','w') as f:
    for afile in files:
        file_path=os.path.join(path,afile)
        if os.path.isfile(file_path):
            if os.path.getsize(file_path)==0:
                continue
            img_name = afile.split('-')[0]
            img_name = img_name.split('.')[0]
            img_name = float(img_name)
            print "img_name: ", img_name
            f.write(str(img_name)+' depth/'+str(img_name)+'.png'+'\n')
        
