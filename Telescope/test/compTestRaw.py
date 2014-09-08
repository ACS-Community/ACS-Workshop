#!/usr/bin/env python

from Acspy.Clients.SimpleClient import PySimpleClient
from time import sleep

client = PySimpleClient("Component Test Client")

##Test database
#database = client.getComponent("DATABASE") 
#proposals = database.getProposals()
#print proposals.pid
#client.releaseComponent("DATABASE")

#Test instrument
instrument = client.getComponent("INSTRUMENT") 
instrument.cameraOn()
instrument.cameraOff()
client.releaseComponent("INSTRUMENT")

#Test telescope
telescope = client.getComponent("TELESCOPE") 
pos = telescope.getCurrentPosition()
print 'Elevation: ', pos.el, 'Azimut: 'pos.az

# Set new coordinates
pos.el = 45.0
pos.az = 90.0
# Move the telescope
telescope.moveTo(pos)
print 'Elevation: ', pos.el, 'Azimut: 'pos.az

for i in range (0, 91):
    pos.el = float(i)
    pos.az = 0.0
    telescope.moveTo(pos)
    print 'Elevation: ', pos.el, 'Azimut: ', pos.az
    sleep(1)

client.releaseComponent("TELESCOPE")

#Test scheduler
scheduler = client.getComponent("SCHEDULER") 
scheduler.start()
scheduler.stop()
client.releaseComponent("SCHEDULER")
