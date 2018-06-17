#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
king@2018.5.12
# Rename a image file name
"""

import os
from scipy.io import loadmat as sio
import numpy as np
import PIL.Image as Image


os.chdir('/home/king/Documents/image/scene_09/comp6_val_cls')
path=os.getcwd()    # /home/king/Documents/image/scene_09/rgb
files=os.listdir(path)  # 00118-color.png
print "files: ", files

for afile in files:
    file_path=os.path.join(path,afile)
    if os.path.isfile(file_path):
        if os.path.getsize(file_path)==0:
            continue
        tmp_img = Image.open(file_path)
        img_name = afile.split('-')[0]
        img_name = img_name.split('.')[0]
        img_name = float(img_name)
        print "img_name: ", img_name
        save_path=os.path.join(path,str(img_name)+'.png')
        tmp_img.save(save_path)
        
