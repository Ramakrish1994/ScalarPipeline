#!/usr/bin/python
from Tkinter import *
import Tkinter
top = Tkinter.Tk()
# Code to add widgets will go here...
fil = open("ins_pipeline.txt",'r')
lines = []
for line in fil:
	lines.append(line.strip())

var = StringVar()
label = Label( top, textvariable=var, relief=RAISED )

var.set("Clock Cycle = "+lines[6])
label.pack()

F = Tkinter.Button(top, text ="Fetch: " + lines[0])
D = Tkinter.Button(top, text ="Decode: " + lines[1])
R = Tkinter.Button(top, text ="Register Read: " + lines[2])
E = Tkinter.Button(top, text ="Execute: " + lines[3])
M = Tkinter.Button(top, text ="Memory Access: " + lines[4])
W = Tkinter.Button(top, text ="Write Back: " + lines[5])

F.pack()
D.pack()
R.pack()
E.pack()
M.pack()
W.pack()

top.mainloop()