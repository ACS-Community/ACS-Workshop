#!/usr/bin/env python

from SORTErrImpl import *
from TYPES import *
from Acspy.Clients.SimpleClient import PySimpleClient
import re

def getProposals():
		print client.obj().getProposals()

def getProposalStatus(args):
		print client.obj().getProposalStatus(args)

def storeProposal(args):
	print client.obj().storeProposal(args)
	
def getProposalObservations(args):
	print client.obj().getProposalObservations(args)

def storeImage(args):
	client.obj().storeImage(args[0], args[1], args[2])

commands= {
	"getProposals": getProposals,
	"getProposalStatus": getProposalStatus,
	"storeProposal": storeProposal,
	"getProposalObservations": getProposalObservations,
	"storeImage": storeImage,
	"quit": exit,
}

class Client():
	def __init__(self):
		self.setUp()
	
	def setUp(self):
		self.client = PySimpleClient()
		self.comp = self.client.getComponent("DATABASE")

	def obj(self):
				return self.comp

	def tearDown(self):
		self.client.releaseComponent("CONSOLE")

def usage():
	print """
getProposals
getProposalStatus pid
storeProposal [Target(tid, Position(az,el), expTime), ...]
getProposalObservations pid
storeImage [pid, tid, [value, value, ...]]
quit
"""

if __name__ == "__main__":
	client = Client()
	usage()
	while True:
		str = raw_input(">>")
		m = re.search(r'(\w+)(([ \t]+)(.*))?', str)
		args = m.group(4)
		cmd = commands[m.group(1)]
		if cmd:
			try:
				if args:
					#print args
					cmd(eval(args))
				else:
					cmd()
			except Exception as ex:
				print ex
			

	client.tearDown()
