# -*- coding: utf-8 -*- 
import os
from PIL import Image, ImageFilter 
from numba import cuda
import numpy as np
from math import *

w=540
h=360
#w=2
#h=2
pixelDx = 2.0/w
ylen = (2.0*h) / w;
offset = np.zeros(3,np.float32)
startPos = np.array([-1.0, -ylen/2, 0.0]) + 0.5*np.array([pixelDx, pixelDx, 0.0]);
@cuda.jit
def cudakernel1(array,re,offset,startPos,ra,rb,sdf,objSDF,traceDis,tDis):
    i,j = cuda.grid(2)
    #print(i,j)
    for k in range(3):
        ra[i,j,k] = offset[k]
        rb[i,j,k] = offset[k] + startPos[k]

        if k==0:
            rb[i,j,k] += i*pixelDx
        elif k== 1:
            rb[i,j,k] += j*pixelDx
        elif k==2 :
            ra[i,j,k] += 1
        re[i,j,k] = rb[i,j,k]-ra[i,j,k]
        tDis[i,j,0] += pow(re[i,j,k],2)

    tDis[i,j,0] = sqrt(tDis[i,j,0])
    
    for k in range(3):
        re[i,j,k] /= tDis[i,j,0]
    
    #for k in range(3):
    #    re[i,j,k]=0
    #re[i,j,2] = -1
    #print(i,j,rb[i,j,0],rb[i,j,1],rb[i,j,2])
    #print(i,j,ra[i,j,0],ra[i,j,1],ra[i,j,2])
    while 1:
        #sdf sphere
        objSDF[i,j,0] = 0
        
        objSDF[i,j,0] += pow(rb[i,j,0],2)
        objSDF[i,j,0] += pow(rb[i,j,1],2)
        objSDF[i,j,0] += pow(rb[i,j,2]+5,2)
        objSDF[i,j,0] = sqrt(objSDF[i,j,0])
        objSDF[i,j,0] -= 1.0
        #choose minSDF
        sdf[i,j,0] = objSDF[i,j,0]
        #???
        #if i==2 and j==135:
            #print(objSDF[i,j,0],rb[i,j,0],rb[i,j,1],rb[i,j,2])
        
        if sdf[i,j,0]>10:
            break;
        elif sdf[i,j,0] <= 0.01:
            traceDis[i,j,0] = sdf[i,j,0]
            #if i<100 :
            #    print(i,j,"traced")
            break;
            
        for k in range(3):
            rb[i,j,k] += sdf[i,j,0]*re[i,j,k]
        
        #print(sdf[i,j,0],rb[i,j,0],rb[i,j,1],rb[i,j,2])
               
array = np.zeros((w,h,3), np.float32)
re = np.zeros((w,h,3), np.float32)
print('Initial array:', array)

#ra = np.zeros(3,np.float32)
ra = np.zeros((w,h,3), np.float32)
#rb = np.zeros(3,np.float32)
rb = np.zeros((w,h,3), np.float32)
objSDF = np.full((w,h,1), -1.0)
sdf = np.full((w,h,1), -1.0)
traceDis = np.full((w,h,1), -1.0)
tDis = np.full((w,h,1), 0.0)
print('Kernel launch...')
cudakernel1[(w,h,1), 1](array,re,offset,startPos,ra,rb,sdf,objSDF,traceDis,tDis)
print('Updated array:', traceDis)

#for i in range(w):
#    for j in range(h):
#        print(i,j,traceDis[i,j,0])
with open('z.txt', 'w') as f:
    f.write(str(w)+' '+str(h)+"\n")
    for j in range(h):
        for i in range(w):
            tt = traceDis[i,j,0]
            if tt < 0 :
                f.write(str(i)+' '+str(j)+" 0 0 0\n")
            else:
                f.write(str(i)+' '+str(j)+" 255 0 0\n")
    f.close()