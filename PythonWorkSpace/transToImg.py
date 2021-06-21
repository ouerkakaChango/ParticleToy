# -*- coding: utf-8 -*- 
import os
from PIL import Image, ImageFilter 
with open("z.txt","r") as f:
    imInfo = f.readline().split(" ")
    w = int(imInfo[0])
    h = int(imInfo[1])
    #print(w,h)
    im = Image.new("RGB",(w,h),(0,0,0));
    count=0;
    #print(len(f.readlines()))
    for line in f.readlines():
        pixelInfo = line.split(" ")
        if len(pixelInfo) == 5:
            #print("aa")
            r = int(pixelInfo[2])
            g = int(pixelInfo[3])
            b = int(pixelInfo[4])
        else:
            r = int(pixelInfo[0])
            g = int(pixelInfo[1])
            b = int(pixelInfo[2])
        x = int(count%w);
        y = int((count-x)/w);
        count+=1
        #print(x,y)
        im.putpixel((x,h-y-1),(r,g,b))
        #if r>1 and x<100:
        #    print(x,y,pixelInfo)
    im.save("z.png");