#!/bin/env python

from Acspy.Clients.SimpleClient import PySimpleClient
from Callback import Callback
from ACS import CBDescIn
from time import sleep

client = PySimpleClient()

one = client.getComponent('OneProperty')
p = one._get_doubleProp01()
p.set_sync(3.1415)
print "Current value: " + str(p.get_sync())

cb = Callback()
cbOffshoot = client.activateOffShoot(cb)
desc = CBDescIn(long(1E7), long(0), 0)

monitor = p.create_monitor(cbOffshoot, desc)

sleep(10.0)

monitor.destroy()

client.releaseComponent('OneProperty')
client.disconnect()

