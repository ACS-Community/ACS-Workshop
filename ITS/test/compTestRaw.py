#!/usr/bin/env python

from Acspy.Clients.SimpleClient import PySimpleClient
import TYPES
import time

client = PySimpleClient("Component Test Client")

#Test database
database = client.getComponent("DATABASE") 

#Test instrument
instrument = client.getComponent("INSTRUMENT") 

#Test telescope
telescope = client.getComponent("TELESCOPE") 

#Test scheduler
scheduler = client.getComponent("SCHEDULER") 

targets = []
for i in range(1,5):
	targets.append(TYPES.Target(i, TYPES.Position(float(i*10), float(i*5)), i*2+1))
database.storeProposal(targets)
targets = []
for i in range(6,7):
	targets.append(TYPES.Target(i, TYPES.Position(float(i*10), float(i*5)), i*2+1))
database.storeProposal(targets)
targets = []
for i in range(8,10):
	targets.append(TYPES.Target(i, TYPES.Position(float(i*10), float(i*5)), i*2+1))
database.storeProposal(targets)
targets = []
for i in range(10,14):
	targets.append(TYPES.Target(i, TYPES.Position(float(i*10), float(i*5)), i*2+1))
database.storeProposal(targets)

scheduler.start()
time.sleep(2)
scheduler.stop()
time.sleep(2)
scheduler.start()
time.sleep(5)
for i in range(10,14):
	targets.append(TYPES.Target(i, TYPES.Position(float(i*10), float(i*5)), i*2+1))
time.sleep(5)
database.storeProposal(targets)
scheduler.stop()
scheduler.start()

client.releaseComponent("DATABASE")
client.releaseComponent("INSTRUMENT")
client.releaseComponent("TELESCOPE")
client.releaseComponent("SCHEDULER")
