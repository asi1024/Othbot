#!/usr/local/bin/python

import Image
import autopy
import os
import time

startX, spanX, startY, spanY = 30, 150, 130, 40
pX, pY = 180, 225
size, span = 30, 56

boxs = size ** 2
starts = spanX * spanY

# draw
def draw_start(im):
    for i in range(spanY):
        for j in range(spanX):
            im.putpixel ((startX + j, startY + i), (0, 0, 0))

def draw_board(im):
    for row in range(8):
        for col in range(8):
            x, y = pX + col * span, pY + row * span
            for i in range(size):
                for j in range(size):
                    im.putpixel ((x + j, y + i), (0, 0, 0))

def draw(im):
    draw_start(im)
    draw_board(im)

#take
def take_start(im):
    R, G, B = 0, 0, 0
    for i in range(spanY):
        for j in range(spanX):
            r, g, b = im.getpixel ((startX + j, startY + i))
            R, G, B = R + r, G + g, B + b
    R, G, B = R / starts, G / starts, B / starts
    return (R + G + B > 430)

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

def output_data(dat):
    for ary in dat:
        for i in ary:
            print i,
        print ""

def board_data(im):
    dat = [[0 for i in range(8)] for j in range(8)]
    for i in range(8):
        for j in range(8):
            dat[i][j] = take_box(im, i, j)
    return dat

def screen_data():
    os.system("mkdir -p tmp")
    os.system("screencapture tmp/sc.png")
    im = Image.open("tmp/sc.png")
    if (take_start(im)):
        autopy.mouse.move(startX + spanX / 2, startY + spanY / 2)
        time.sleep(2)
        autopy.mouse.click()
        time.sleep(2)
        autopy.mouse.click()
    output_data(board_data(im))

screen_data()
