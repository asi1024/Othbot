#!/usr/local/bin/python

import Image
import autopy
import os

pX = 170
pY = 150
size = 50
span = 56

def draw_box(im, a, b):
    for i in range(size):
        for j in range(size):
            im.putpixel ((a + j, b + i), (0, 0, 0))

def screen_data():
    os.system("mkdir -p tmp")
    os.system("screencapture tmp/sc.png")
    im = Image.open("tmp/sc.png")
    for i in range(8):
        for j in range(8):
            draw_box(im, pX + j * span, pY + i * span)
    im.show()

screen_data()
