#!/usr/local/bin/python

import Image
import autopy
import os
import time

pX, pY = 180, 225
size, span = 30, 56

boxs = size * size

def draw_box(im, row, col):
    x, y = pX + col * span, pY + row * span
    for i in range(size):
        for j in range(size):
            im.putpixel ((x + j, y + i), (0, 0, 0))

def take_box(im, row, col):
    x, y = pX + col * span, pY + row * span
    R, G, B = 0, 0, 0
    for i in range(size):
        for j in range(size):
            r, g, b = im.getpixel ((x + j, y + i))
            R, G, B = R + r, G + g, B + b
    R, G, B = R / boxs, G / boxs, B / boxs
    if G - B > 10:
        return 0
    elif B > 200:
        return 3
    elif B > 105:
        return 4
    elif R > 53:
        return 2
    else:
        return 1

def screen_data():
    os.system("mkdir -p tmp")
    os.system("screencapture tmp/sc.png")
    im = Image.open("tmp/sc.png")
    for i in range(8):
        for j in range(8):
            p = take_box(im, i, j)
            print p, " ",
        print ""

screen_data()
