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

@cuda.jit
def cudakernel1(needTrace,orirb,re,rb,sdf,objSDF,traceDis,traceObj,tInt,tVec):
    i,j = cuda.grid(2)
    #print(i,j)
    if needTrace[i,j,0] == 0:
        return;
   
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
        
        #sdf lightBox center:(0.0, 3.9, -5.0),bound:(0.8,0.08,0.8)
        objSDF[i,j,2] = 0
        tVec[i,j,0] = abs(rb[i,j,0]-0.0)-0.8
        tVec[i,j,1] = abs(rb[i,j,1]-3.9)-0.08
        tVec[i,j,2] = abs(rb[i,j,2]+5.0)-0.8
        for k in range(3):
            tVec[i,j,k] = max(tVec[i,j,k], 0.0)
        objSDF[i,j,2] += pow(tVec[i,j,0],2)
        objSDF[i,j,2] += pow(tVec[i,j,1],2)
        objSDF[i,j,2] += pow(tVec[i,j,2],2)
        objSDF[i,j,2] = sqrt(objSDF[i,j,2])
        objSDF[i,j,2] += min(max(tVec[i,j,0], max(tVec[i,j,1], tVec[i,j,2])), 0.0);
        
        #choose minSDF
        sdf[i,j,0] = objSDF[i,j,0]
        tInt[i,j,0] = 0
        for objInx in range(OBJNUM):
            if objSDF[i,j,objInx] < sdf[i,j,0]:
                sdf[i,j,0] = objSDF[i,j,objInx]
                tInt[i,j,0] = objInx
            
        #judge hit
        if sdf[i,j,0]>MAXDIS:
            break;
        elif sdf[i,j,0] <= TRACETHRE and sdf[i,j,0] >= -TRACETHRE:
            #trace hit !!!
            #traceDis[i,j,0] = sdf[i,j,0]
            traceDis[i,j,0] = 0
            traceDis[i,j,0] += pow(rb[i,j,0]-orirb[i,j,0],2)
            traceDis[i,j,0] += pow(rb[i,j,1]-orirb[i,j,1],2)
            traceDis[i,j,0] += pow(rb[i,j,2]-orirb[i,j,2],2)
            traceDis[i,j,0] = sqrt(traceDis[i,j,0])
            
            traceObj[i,j,0] = tInt[i,j,0]
            break;
            
         #trace along
        for k in range(3):
            rb[i,j,k] += sdf[i,j,0]*re[i,j,k]
        #print(sdf[i,j,0],rb[i,j,0],rb[i,j,1],rb[i,j,2])
               

#print(str(sys.argv[1]))
with open("CalculationCacheSpace\\"+str(sys.argv[1]),"r") as f:
    imInfo = f.readline().split(" ")
    w = int(imInfo[0])
    h = int(imInfo[1])
    print(w,h)
    re = np.zeros((w,h,3), np.float32)
    rb = np.zeros((w,h,3), np.float32)
    orirb = np.zeros((w,h,3), np.float32)
    needTrace = np.full((w,h,1), 1)
    # read rb,r3 from req.txt
    count=0
    for line in f.readlines():
        pixelInfo = line.split(" ")
        x = int(count%w);
        y = int((count-x)/w);
        if len(pixelInfo) == 6:
            rb[x,y,0] = float(pixelInfo[0])
            rb[x,y,1] = float(pixelInfo[1])
            rb[x,y,2] = float(pixelInfo[2])
            re[x,y,0] = float(pixelInfo[3])
            re[x,y,1] = float(pixelInfo[4])
            re[x,y,2] = float(pixelInfo[5])
            #trace STARTLEN
            for k in range(3):
                rb[x,y,k] += STARTLEN*re[x,y,k]
            #print(STARTLEN)
            orirb[x,y,0] = rb[x,y,0]
            orirb[x,y,1] = rb[x,y,1]
            orirb[x,y,2] = rb[x,y,2]

        else :
            needTrace[x,y,0]=0
        
        #print(rb[x,y,0],rb[x,y,1],rb[x,y,2])
        #print(re[x,y,0],re[x,y,1],re[x,y,2])
        #print(x,y)
        count+=1
        
    objSDF = np.full((w,h,OBJNUM), -1.0)
    
    sdf = np.full((w,h,1), -1.0)
    traceDis = np.full((w,h,1), -1.0)
    tFloat = np.full((w,h,1),MAXDIS)
    
    traceObj = np.full((w,h,1), -1)
    tInt = np.full((w,h,1), -1)
    
    tVec = np.full((w,h,3), 0.0)
    print('Kernel launch...')
    cudakernel1[(w,h,1), 1](needTrace,orirb,re,rb,sdf,objSDF,traceDis,traceObj,tInt,tVec)
    #print('Updated array:', traceDis)

    with open('z.txt', 'w') as f:
        f.write(str(w)+' '+str(h)+"\n")
        for j in range(h):
            for i in range(w):
                tt = traceDis[i,j,0]
                tobj = traceObj[i,j,0]
                if tt < 0 :
                    f.write(str(i)+' '+str(j)+" 0 0 0\n")
                else:
                    if tobj<0:
                        print("ATTENTION: error")
                    elif tobj == 2:
                        f.write(str(i)+' '+str(j)+" 255 255 255\n")
                    else :
                        f.write(str(i)+' '+str(j)+" 255 0 0\n")
        f.close()