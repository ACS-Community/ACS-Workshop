#!/usr/bin/env python

"""
Test very simple T1 components.
"""

from Acspy.Clients.SimpleClient import PySimpleClient
import TYPES

simpleClient = PySimpleClient()

try:
    #test an immortal cob first
    print "Testing T1_TEST"
    comp = simpleClient.getComponent("T1_TEST")
    survival = TYPES.Position(90,15)
    comp.moveTo(survival)
    
    print comp.getCurrentPosition()
    
    target = TYPES.Position(45,45)
    imageList = comp.observe(target,5)
    
    print comp.getCurrentPosition()

    if imageList != "":
        print "Image received"
    else:
        print "Test failed! No image received."    


except Exception, e:
    print "Test FAILED!!!"
    print "The exception was:", e

