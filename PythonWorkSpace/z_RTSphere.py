# -*- coding: utf-8 -*- 
import os
from PIL import Image, ImageFilter 
from numba import cuda
import numpy as np
from math import *

w=1080
h=720

screenPos = np.zeros(3)
dx = 2.0 / w;
ylen = dx*h;
screenLeftDownPos = np.array([-1.0, -ylen/2, 0.0]);
screenLeftDownPos += 0.5*np.array([dx, dx, 0.0]);
#print(screenLeftDownPos);

@cuda.jit
def kernel_initRays(rayPos,rayDir):
    i,j = cuda.grid(2)
    rayPos[i,j,0] = screenPos[0] + screenLeftDownPos[0] + i*dx
    rayPos[i,j,1] = screenPos[1] + screenLeftDownPos[1] + j*dx
    rayPos[i,j,2] = screenPos[2] + screenLeftDownPos[2]
    
    #rayDir
    rayDir[i,j,0] = rayPos[i,j,0] - screenPos[0]
    rayDir[i,j,1] = rayPos[i,j,1] - screenPos[1]
    rayDir[i,j,2] = rayPos[i,j,2] - screenPos[2] - 1

@cuda.jit
def kernel_SphereSDFTrace(rayPos,rayDir,objSDF,sdf,traceDis,tDis,tVec):
    i,j = cuda.grid(2)
    #print(i,j)
   
    while 1:
        #sdf sphere
        objSDF[i,j,0] = 0
        
        objSDF[i,j,0] += pow(rayPos[i,j,0],2)
        objSDF[i,j,0] += pow(rayPos[i,j,1],2)
        objSDF[i,j,0] += pow(rayPos[i,j,2]+5,2)
        objSDF[i,j,0] = sqrt(objSDF[i,j,0])
        objSDF[i,j,0] -= 1.0
        
        #sdf box1 center:(0,-1.2,-5),bound:(5.0, 0.1, 5.0)
        objSDF[i,j,1] = 0
        #1.1 q = abs(p-center) - bound;
        tVec[i,j,0] = abs(rayPos[i,j,0]-0.0)-5.0
        tVec[i,j,1] = abs(rayPos[i,j,1]+1.2)-0.1
        tVec[i,j,2] = abs(rayPos[i,j,2]+5.0)-5.0
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
            #print(objSDF[i,j,0],rayPos[i,j,0],rayPos[i,j,1],rayPos[i,j,2])
        
        if sdf[i,j,0]>10:
            break;
        elif sdf[i,j,0] <= 0.01:
            traceDis[i,j,0] = sdf[i,j,0]
            #if i<100 :
            #    print(i,j,"traced")
            break;
            
        for k in range(3):
            rayPos[i,j,k] += sdf[i,j,0]*rayDir[i,j,k]
        
        #print(sdf[i,j,0],rayPos[i,j,0],rayPos[i,j,1],rayPos[i,j,2])
               

#with open("CalculationCacheSpace\\traceReq0.txt","r") as f:
#    imInfo = f.readline().split(" ")
#    w = int(imInfo[0])
#    h = int(imInfo[1])
#    print(w,h)
#    rayDir = np.zeros((w,h,3), np.float32)
#    rayPos = np.zeros((w,h,3), np.float32)
#    #!!! read rayDir,rayPos from req.txt
#    count=0
#    for line in f.readlines():
#        pixelInfo = line.split(" ")
#        x = int(count%w);
#        y = int((count-x)/w);
#        rayPos[x,y,0] = float(pixelInfo[0])
#        rayPos[x,y,1] = float(pixelInfo[1])
#        rayPos[x,y,2] = float(pixelInfo[2])
#        rayDir[x,y,0] = float(pixelInfo[3])
#        rayDir[x,y,1] = float(pixelInfo[4])
#        rayDir[x,y,2] = float(pixelInfo[5])
#        
#        #print(rayPos[x,y,0],rayPos[x,y,1],rayPos[x,y,2])
#        #print(rayDir[x,y,0],rayDir[x,y,1],rayDir[x,y,2])
#        #print(x,y)
#        count+=1

rayDir = np.zeros((w,h,3), np.float32)
rayPos = np.zeros((w,h,3), np.float32)
print('Init Kernel launch...')
kernel_initRays[(w,h,1), 1](rayPos,rayDir)
    
worlNorm = np.zeros((w,h,3), np.float32)
objSDF = np.full((w,h,2), -1.0)
sdf = np.full((w,h,1), -1.0)
traceDis = np.full((w,h,1), -1.0)
tDis = np.full((w,h,1), 0.0)
tVec = np.full((w,h,3), 0.0)
print('SphereSDFTrace Kernel launch...')
kernel_SphereSDFTrace[(w,h,1), 1](rayPos,rayDir,objSDF,sdf,traceDis,tDis,tVec)
#print('Updated array:', traceDis)
    #
    ##for i in range(w):
    ##    for j in range(h):
    ##        print(i,j,traceDis[i,j,0])
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