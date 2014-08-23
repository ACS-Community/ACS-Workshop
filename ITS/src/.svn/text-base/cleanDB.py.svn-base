#!/usr/bin/env python
import unittest
import time, logging, random
from SORTErr import *
import TYPES
from Acspy.Clients.SimpleClient import PySimpleClient

client = PySimpleClient()
db = client.getComponent("DATABASE")
db.clean()
client.releaseComponent("DATABASE")
