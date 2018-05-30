import cv2,os
import numpy as np
from PIL import Image 
import pickle
import serial
import speech_recognition as sr
import struct

# FAce REcog
recognizer = cv2.face.LBPHFaceRecognizer_create()
recognizer.read('trainer/trainer.yml')
cascadePath = "Classifiers/face.xml"
faceCascade = cv2.CascadeClassifier(cascadePath);
#print(faceCascade.load(cascadePath))
path = 'dataSet'

cam = cv2.VideoCapture(0)
font =cv2.FONT_HERSHEY_SIMPLEX #Creates a font
fontscale = 1
fontcolor = (255,255,255)

#----------





def detectv():
    ard.write(b'B')





    
ard = serial.Serial('/dev/ttyACM0',9600,timeout=.1)
ard.flush()
#ard.flushInput()
while True :
    #ard.flushInput()
    data = ard.readline()[:-2]
    if data :
        print (str(data))
        if 'V' in str(data):
            
            r = sr.Recognizer()
            with sr.Microphone() as source:
                
                print("Password : ")
                audio = r.listen(source)
            
               # print("Google Speech Recognition thinks you said " + r.recognize_google(audio))
                passWD = r.recognize_google(audio)
                print(passWD)
                if passWD == '1700':    
                    print('True')
                    detectv()
                    ard.flush()
                    #ard.write(struct.pack('>B',5)
                    #ard.write(b'A')
                else :
                     ard.write(b'E')


        elif 'CAM' in str(data):
                flag = 0;
                while True:
                    
                    
                    ret, im =cam.read()
   

                    gray=cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)
                    faces=faceCascade.detectMultiScale(gray, scaleFactor=1.2, minNeighbors=5, minSize=(30, 30), flags=cv2.CASCADE_SCALE_IMAGE)
   
                    print(faces)

                    for(x,y,w,h) in faces:
                        
                        print('test')
                        nbr_predicted, conf = recognizer.predict(gray[y:y+h,x:x+w])
        
                        cv2.rectangle(im,(x-50,y-50),(x+w+50,y+h+50),(225,0,0),2)
        
                        if(nbr_predicted==1111):
                             nbr_predicted='Zika'
                             detectv()
                             detectv()
                             data = ''
                             flag = 1
                             break
                             
                        else :
                             ard.write(b'E')
                             break
                       # cv2.cv.PutText(cv2.cv.fromarray(im),str(nbr_predicted)+"--"+str(conf), (x,y+h),font, 255) #Draw the text
                        cv2.putText(im,str(nbr_predicted),(x,y+h),font,fontscale,fontcolor)
                        print('test')
                        cv2.imshow('im',im)
                       
                       # cv2.waitKey(10)

                
                    if(flag == 1):
                        break
                    


