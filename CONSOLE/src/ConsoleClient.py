#!/usr/bin/env python

from Acspy.Clients.SimpleClient import PySimpleClient

print "The following methods are available"
print "getMode, setMode"
print "cameraOn, cameraOff"

def help():
    print "To set Mode, please enter ' Mode 1' for auto or 0"

def setMode(args):
    arg1 = (int(args[0]) != 0)
    print arg1
    comp.setMode(arg1)

def getMode(args):
    print comp.getMode()
    
def cameraOn(args):
    print args
    comp.cameraOn()
    
def cameraOff(args):
    print args
    comp.cameraOff()
    
def setRGB(args):
    print args
    comp.setRGB(args)

def getCameraImage(args):
    print args
    comp.getCameraImage()

def setPixelBias(args):
    print args
    comp.setPixelBias(args)

def setResetLevel(args):
    print args
    comp.setResetLevel()

def moveTelescope(args):
    print args
    comp.moveTelescope(args)
    
def getTelescopePosition(args):
    print comp.getTelescopePosition()


commands = {
    "setMode": setMode,
    "getMode": getMode,
    "cameraOn": cameraOn,
    "cameraOff": cameraOff,
    "getCameraImage": getCameraImage,
    "setPixelBias": setPixelBias,
    "setResetLevel": setResetLevel,
    "moveTelescope": moveTelescope,
    "getTelescopePosition": getTelescopePosition,
    "quit": exit
}


if __name__ == "__main__":
    client = PySimpleClient()
    comp = client.getComponent("CONSOLE")

    while True:
        str = raw_input("#> ")
        words = str.split(" ")
        command = commands[words[0]]
        if command:
            command(words[1:])

