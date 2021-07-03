# -*- coding: utf-8 -*- 
import os
import sys
from PIL import Image, ImageFilter 
from numba import cuda
import numpy as np
from math import *

MAXDIS=100
TRACETHRE=0.01
OBJNUM=3
STARTLEN=0.1
w=540
h=360


with open("CalculationCacheSpace\\"+str(sys.argv[1]),"r") as f:
    imInfo = f.readline().split(" ")
    w = int(imInfo[0])
    h = int(imInfo[1])
    print(w,h)
    objInx = np.full((w,h,1), -1)
    count=0
    for line in f.readlines():
        pixelInfo = line.split(" ")
        x = int(count%w);
        y = int((count-x)/w);
        #traceDis = float(pixelInfo[0])
        objInx[x,y,0] = int(pixelInfo[1])
        #print(pixelInfo[1])
        count+=1
        
    with open('z.txt', 'w') as f:
        f.write(str(w)+' '+str(h)+"\n")
        for j in range(h):
            for i in range(w):
                if objInx[i,j,0]==2:
                    f.write(str(i)+' '+str(j)+" 255 255 255\n")
                else :
                        f.write(str(i)+' '+str(j)+" 255 0 0\n")