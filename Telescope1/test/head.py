#!/usr/bin/env python

"""
Test very simple T1 components.
"""

from Acspy.Clients.SimpleClient import PySimpleClient
import TYPES

simpleClient = PySimpleClient()

#test an immortal cob first
telescope = simpleClient.getComponent("TELESCOPE1")

