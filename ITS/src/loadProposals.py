#!/usr/bin/env python
import unittest
import time, logging, random
from SORTErr import *
import TYPES
from Acspy.Clients.SimpleClient import PySimpleClient


client = PySimpleClient()
db = client.getComponent("DATABASE")

for p in range(100):
    targets = []
    for i in range(int(random.random() * 10)):
        az = random.random() * 360.0
        el = random.random() * 180.0 - 90.0
        target = TYPES.Target(i+1, TYPES.Position(az, el), int(random.random() * 10))
        targets.append(target)
        db.storeProposal(targets)

client.releaseComponent("DATABASE")
