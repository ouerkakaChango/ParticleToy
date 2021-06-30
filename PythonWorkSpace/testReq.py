# -*- coding: utf-8 -*- 
import os
from PIL import Image, ImageFilter 
from numba import cuda
import numpy as np
from math import *

w=540
h=360

@cuda.jit
def cudakernel1(re,rb,sdf,objSDF,traceDis,tDis,tVec):
    i,j = cuda.grid(2)
    #print(i,j)
   
    while 1:
        #sdf sphere
        objSDF[i,j,0] = 0
        
        objSDF[i,j,0] += pow(rb[i,j,0],2)
        objSDF[i,j,0] += pow(rb[i,j,1],2)
        objSDF[i,j,0] += pow(rb[i,j,2]+5,2)
        objSDF[i,j,0] = sqrt(objSDF[i,j,0])
        objSDF[i,j,0] -= 1.0
        
        #sdf box1 center:(0,-1.2,-5),bound:(5.0, 0.1, 5.0)
        objSDF[i,j,1] = 0
        #1.1 q = abs(p-center) - bound;
        tVec[i,j,0] = abs(rb[i,j,0]-0.0)-5.0
        tVec[i,j,1] = abs(rb[i,j,1]+1.2)-0.1
        tVec[i,j,2] = abs(rb[i,j,2]+5.0)-5.0
        #1.2 len(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
        for k in range(3):
            tVec[i,j,k] = max(tVec[i,j,k], 0.0)
        objSDF[i,j,1] += pow(tVec[i,j,0],2)
        objSDF[i,j,1] += pow(tVec[i,j,1],2)
        objSDF[i,j,1] += pow(tVec[i,j,2],2)
        objSDF[i,j,1] = sqrt(objSDF[i,j,1])
        objSDF[i,j,1] += min(max(tVec[i,j,0], max(tVec[i,j,1], tVec[i,j,2])), 0.0);
        #choose minSDF
        sdf[i,j,0] = min(objSDF[i,j,0],objSDF[i,j,1])
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
               

with open("CalculationCacheSpace\\traceReq0.txt","r") as f:
    imInfo = f.readline().split(" ")
    w = int(imInfo[0])
    h = int(imInfo[1])
    print(w,h)
    re = np.zeros((w,h,3), np.float32)
    rb = np.zeros((w,h,3), np.float32)
    #!!! read re,rb from req.txt
    count=0
    for line in f.readlines():
        pixelInfo = line.split(" ")
        x = int(count%w);
        y = int((count-x)/w);
        rb[x,y,0] = float(pixelInfo[0])
        rb[x,y,1] = float(pixelInfo[1])
        rb[x,y,2] = float(pixelInfo[2])
        re[x,y,0] = float(pixelInfo[3])
        re[x,y,1] = float(pixelInfo[4])
        re[x,y,2] = float(pixelInfo[5])
        
        #print(rb[x,y,0],rb[x,y,1],rb[x,y,2])
        #print(re[x,y,0],re[x,y,1],re[x,y,2])
        #print(x,y)
        count+=1
        
    
    objSDF = np.full((w,h,2), -1.0)
    sdf = np.full((w,h,1), -1.0)
    traceDis = np.full((w,h,1), -1.0)
    tDis = np.full((w,h,1), 0.0)
    tVec = np.full((w,h,3), 0.0)
    print('Kernel launch...')
    cudakernel1[(w,h,1), 1](re,rb,sdf,objSDF,traceDis,tDis,tVec)
    #print('Updated array:', traceDis)
    
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