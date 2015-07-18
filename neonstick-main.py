import pygame
import time
import serial
import numpy as np
import wave
import csv
import random
port='COM12'
baudrate=9600
ser=serial.Serial(port=port,baudrate=baudrate)

music=wave.open('dubstep.wav', mode='rb')
params = music.getparams()
nchannels, sampwidth, framerate, nframes = params[:4]
str_data = music.readframes(nframes)
music.close()

wave_data = np.fromstring(str_data, dtype=np.short)
wave_data.shape = -1, 2
wave_data = wave_data.T
time_array = np.arange(0, nframes) * (1.0 / framerate)
pts=zip(9*time_array[::1000],300+wave_data[0][::1000]/200)



number=0

color=[(100,100,100),(255,50,50),(50,255,50),(50,50,255),(255,50,255),(255,255,50),(50,255,255),(255,100,0),(0,255,100),(100,0,255)]

def send_signal(time,timing):
    if(time>int(timing[number][0])):
        global number
        ser.write(timing[number][1])
        print timing[number][1]
        number+=1

def blit_clue_words(surface):
    font=pygame.font.Font(None,30)
    exp0=font.render('0:Silence',0,color[0])
    exp1=font.render('1:Solidcolor',0,color[1])
    exp2=font.render('2:Blinkcolor',0,color[2])
    exp3=font.render('3:Dual_Solidcolor',0,color[3])
    exp4=font.render('4:Dual_Blinkcolor',0,color[4])
    exp5=font.render('5:Spiral_0',0,color[5])
    exp6=font.render('6:Spiral_pi',0,color[6])
    exp7=font.render('7:Random',0,color[7])
    exp8=font.render('8:Rainbow',0,color[8])
    exp9=font.render('9:DoubleRainbow',0,color[9])
    surface.blit(exp0,(10,10))
    surface.blit(exp1,(150,10))
    surface.blit(exp2,(300,10))
    surface.blit(exp3,(450,10))
    surface.blit(exp4,(700,10))
    surface.blit(exp5,(10,50))
    surface.blit(exp6,(150,50))
    surface.blit(exp7,(300,50))
    surface.blit(exp8,(450,50))
    surface.blit(exp9,(600,50))

def blit_menu_buttons(surface):
    pygame.draw.rect(surface,(100,0,0),(100,200,300,200))
    pygame.draw.rect(surface,(0,100,0),(400,200,300,200))
    pygame.draw.rect(surface,(0,0,100),(700,200,300,200))
    pygame.draw.rect(surface,(0,100,100),(1000,200,300,200))
    font=pygame.font.Font(None,60)
    read=font.render('Load',0,(240,240,240))
    write=font.render('Create',0,(240,240,240))
    realtime=font.render('RealTime',0,(240,240,240))
    edit=font.render('Edit',0,(240,240,240))
    surface.blit(read,(150,280))
    surface.blit(write,(450,280))
    surface.blit(realtime,(750,280))
    surface.blit(edit,(1050,280))

if __name__ == '__main__':
    pygame.init()
    screen=pygame.display.set_mode([1500,600])
    blit_menu_buttons(screen)
    pygame.display.flip()
    menu=True
    READ=False
    WRITE=False
    REAL=False
    EDIT=False
    while menu:
        for event in pygame.event.get():
            if event.type ==pygame.MOUSEBUTTONDOWN and event.button==1:
                if 100<event.pos[0]<400 and 200<event.pos[1]<400:
                    menu=False
                    READ=True
                if 400<event.pos[0]<700 and 200<event.pos[1]<400:
                    menu=False
                    WRITE=True
                if 700<event.pos[0]<1000 and 200<event.pos[1]<400:
                    menu=False
                    REAL=True
                if 1000<event.pos[0]<1300 and 200<event.pos[1]<400:
                    menu=False
                    EDIT=True


    background=pygame.Surface((1500,600))
    pygame.draw.lines(background,(205,250,200),False,pts,1)
    blit_clue_words(background)
    file_name='tempo_1.csv'
    if READ:
        tempo=open(file_name, 'rb')
        spamreader = csv.reader(tempo, delimiter=' ', quotechar='|')
        timing=[[int(row[0]),row[1]] for row in spamreader]
        timing=sorted(timing)
        print timing
        for row in timing:
            pygame.draw.line(background,color[int(row[1][0])],(int(row[0])*9/1000,500),(int(row[0])*9/1000,550),1)
    elif WRITE:
        tempo=open(file_name, 'wb')
        spamwriter = csv.writer(tempo, delimiter=' ',quotechar='|', quoting=csv.QUOTE_MINIMAL)
    elif EDIT:
        tempo=open(file_name, 'rb')
        spamreader = csv.reader(tempo, delimiter=' ', quotechar='|')
        timing=[row for row in spamreader]
        timing=sorted(timing)
        for row in timing:
            pygame.draw.line(background,color[int(row[1][0])],(int(row[0])*9/1000,500),(int(row[0])*9/1000,550),1)
        tempo.close()
        tempo=open(file_name, 'ab')
        spamwriter = csv.writer(tempo, delimiter=' ',quotechar='|', quoting=csv.QUOTE_MINIMAL)

    screen.blit(background,(0,0))
    pygame.display.flip()
    pygame.mixer.music.load("dubstep.mp3")
    pygame.mixer.music.play()
    going=True
    pause=False
    pos=0
    while going :
        screen.blit(background,(0,0))
        pygame.draw.line(screen,(150,150,150),(pygame.mixer.music.get_pos()*9/1000,0),(pygame.mixer.music.get_pos()*9/1000,600),2)
        if READ:
            try:
                send_signal(pygame.mixer.music.get_pos(),timing)
            except:
                pass

        for event in pygame.event.get():
            if event.type ==pygame.KEYDOWN and event.key==pygame.K_ESCAPE:
                going =False
            if event.type ==pygame.KEYDOWN and event.key==pygame.K_SPACE:
                if pause:
                    pygame.mixer.music.unpause()
                    pause=False
                else:
                    pygame.mixer.music.pause()
                    pause=True
            elif event.type ==pygame.KEYDOWN and (48<=event.key<=57 or 97<=event.key<=122):
                if not READ:
                    pygame.draw.line(background,color[event.key-48],(pygame.mixer.music.get_pos()*9/1000,500),(pygame.mixer.music.get_pos()*9/1000,550),1)
                    if REAL:
                        try:
                            ser.write(str(unichr(event.key)))
                        except:
                            pass
                    else:
                        spamwriter.writerow([pygame.mixer.music.get_pos(),str(unichr(event.key))])
        pygame.display.flip()
    ser.close()
    pygame.quit()
