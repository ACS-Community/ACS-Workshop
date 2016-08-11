#!/usr/bin/env python
import unittest, logging
from TYPES import *
from SCHEDULER_MODULE import *
from Acspy.Clients.SimpleClient import PySimpleClient
from SYSTEMErr import NoProposalExecutingEx
import time

class IntegrationTest(unittest.TestCase):
    def setUp(self):
        self.client = PySimpleClient()
	time.sleep(5)
        self.retrieveResources()

    def tearDown(self):
        self.releaseResources()

    def retrieveResources(self):
	# EXTERNAL COMPONENTS
        self.mount = self.client.getComponent('MOUNT')
        self.camera = self.client.getComponent('CAMERA')
        self.storage = self.client.getComponent('STORAGE')
	# IMPLEMENTED COMPONENTS
        self.instrument = self.client.getComponent('INSTRUMENT1')
        self.telescope = self.client.getComponent('TELESCOPE1')
        self.scheduler = self.client.getComponent('SCHEDULER1')
        self.database = self.client.getComponent('DATABASESIM')
        self.console = self.client.getComponent('CONSOLE1')

    def releaseResources(self):
        self.client.releaseComponent(self.mount.name())
        self.client.releaseComponent(self.camera.name())
        self.client.releaseComponent(self.storage.name())
        self.client.releaseComponent(self.database.name())
        self.client.releaseComponent(self.telescope.name())
        self.client.releaseComponent(self.instrument.name())
        self.client.releaseComponent(self.scheduler.name())
        self.client.releaseComponent(self.console.name())

    def getTargets(self):
        targets = TargetList()
        targets.length(2)
        tar = Target()
        tar.tid = 0
        pos = Position()
        pos.az = 100
        pos.el = 60
        tar.coordinates = pos
        tar.expTime = 5
        targets[0] = tar

        tar = Target()
        tar.tid = 1
        pos = Position()
        pos.az = 40
        pos.el = 65
        tar.coordinates = pos
        tar.expTime = 4
        targets[1] = tar
        return targets
        
    #setTo(alt,az) #Moves to required position, unless uncalibrated.
    #offSet(alt,az) #Adds offset movement to each axis, unless uncalibrated.
    #zenith() #Moves the telescope to the zenith, unless uncalibrated.
    #park() #Moves the telescope to the park position, unless uncalibrated.
    #setUncalibrated() #Sets the telescope as uncalibrated.
    #calibrateEnconders() #Calibrates the telescope.
    #commandedAltitude()
    #commandedAzimuth()
    #actualAltitude()
    #actualAzimuth()
    #status()
    def testTelescopeControl(self):
        #Test initial values
        self.assertEquals(self.mount.actualAltitude(), 0)
        self.assertEquals(self.mount.actualAzimuth(), 0)
        self.assertEquals(self.mount.commandedAltitude(), 0)
        self.assertEquals(self.mount.commandedAzimuth(), 0)
        #Test (lack of) movement with uncalibrated encoders
        self.assertEquals(self.mount.status(), 0)
        self.mount.setTo(100, 60)
        self.assertEquals(self.mount.commandedAltitude(), 0)
        self.assertEquals(self.mount.commandedAzimuth(), 0)
        self.assertAlmostEquals(self.mount.actualAltitude(), 0, delta=1)
        self.assertAlmostEquals(self.mount.actualAzimuth(), 0, delta=1)
        self.mount.offSet(5, 5)
        self.assertEquals(self.mount.commandedAltitude(), 0)
        self.assertEquals(self.mount.commandedAzimuth(), 0)
        self.assertAlmostEquals(self.mount.actualAltitude(), 0, delta=1)
        self.assertAlmostEquals(self.mount.actualAzimuth(), 0, delta=1)
        self.mount.offSet(-5, -5)
        self.assertEquals(self.mount.commandedAltitude(), 0)
        self.assertEquals(self.mount.commandedAzimuth(), 0)
        self.assertAlmostEquals(self.mount.actualAltitude(), 0, delta=1)
        self.assertAlmostEquals(self.mount.actualAzimuth(), 0, delta=1)
        self.mount.zenith()
        self.assertEquals(self.mount.commandedAltitude(), 0)
        self.assertEquals(self.mount.commandedAzimuth(), 0)
        self.assertAlmostEquals(self.mount.actualAltitude(), 0, delta=1)
        self.assertAlmostEquals(self.mount.actualAzimuth(), 0, delta=1)
        self.mount.park()
        self.assertEquals(self.mount.commandedAltitude(), 0)
        self.assertEquals(self.mount.commandedAzimuth(), 0)
        self.assertAlmostEquals(self.mount.actualAltitude(), 0, delta=1)
        self.assertAlmostEquals(self.mount.actualAzimuth(), 0, delta=1)
        #Calibrate encoders
        self.mount.calibrateEncoders()
        self.assertEquals(self.mount.status(), 1)
        self.mount.setTo(60, 100)
        self.assertEquals(self.mount.commandedAltitude(),60)
        self.assertEquals(self.mount.commandedAzimuth(), 100)
        self.assertAlmostEquals(self.mount.actualAltitude(), 60, delta=1)
        self.assertAlmostEquals(self.mount.actualAzimuth(), 100, delta=1)
        self.mount.offSet(5, 5)
        self.assertEquals(self.mount.commandedAltitude(),65)
        self.assertEquals(self.mount.commandedAzimuth(), 105)
        self.assertAlmostEquals(self.mount.actualAltitude(), 65, delta=1)
        self.assertAlmostEquals(self.mount.actualAzimuth(), 105, delta=1)
        self.mount.offSet(-5, -5)
        self.assertEquals(self.mount.commandedAltitude(),60)
        self.assertEquals(self.mount.commandedAzimuth(), 100)
        self.assertAlmostEquals(self.mount.actualAltitude(), 60, delta=1)
        self.assertAlmostEquals(self.mount.actualAzimuth(), 100, delta=1)
        self.mount.zenith()
        self.assertEquals(self.mount.commandedAltitude(),90)
        self.assertEquals(self.mount.commandedAzimuth(), 0)
        self.assertAlmostEquals(self.mount.actualAltitude(), 90, delta=1)
        self.assertAlmostEquals(self.mount.actualAzimuth(), 0, delta=1)
        self.mount.park()
        self.assertEquals(self.mount.commandedAltitude(),0)
        self.assertEquals(self.mount.commandedAzimuth(), 0)
        self.assertAlmostEquals(self.mount.actualAltitude(), 0, delta=1)
        self.assertAlmostEquals(self.mount.actualAzimuth(), 0, delta=1)
        #Set encoders uncalibrated
        self.mount.setUncalibrated()
        self.assertEquals(self.mount.status(), 0)
        self.mount.setTo(100, 60)
        self.assertEquals(self.mount.commandedAltitude(), 0)
        self.assertEquals(self.mount.commandedAzimuth(), 0)
        self.assertAlmostEquals(self.mount.actualAltitude(), 0, delta=1)
        self.assertAlmostEquals(self.mount.actualAzimuth(), 0, delta=1)
        self.mount.offSet(5, 5)
        self.assertEquals(self.mount.commandedAltitude(), 0)
        self.assertEquals(self.mount.commandedAzimuth(), 0)
        self.assertAlmostEquals(self.mount.actualAltitude(), 0, delta=1)
        self.assertAlmostEquals(self.mount.actualAzimuth(), 0, delta=1)
        self.mount.offSet(-5, -5)
        self.assertEquals(self.mount.commandedAltitude(), 0)
        self.assertEquals(self.mount.commandedAzimuth(), 0)
        self.assertAlmostEquals(self.mount.actualAltitude(), 0, delta=1)
        self.assertAlmostEquals(self.mount.actualAzimuth(), 0, delta=1)
        self.mount.zenith()
        self.assertEquals(self.mount.commandedAltitude(), 0)
        self.assertEquals(self.mount.commandedAzimuth(), 0)
        self.assertAlmostEquals(self.mount.actualAltitude(), 0, delta=1)
        self.assertAlmostEquals(self.mount.actualAzimuth(), 0, delta=1)
        self.mount.park()
        self.assertEquals(self.mount.commandedAltitude(), 0)
        self.assertEquals(self.mount.commandedAzimuth(), 0)
        self.assertAlmostEquals(self.mount.actualAltitude(), 0, delta=1)
        self.assertAlmostEquals(self.mount.actualAzimuth(), 0, delta=1)


#    def testCamera(self):
#        pass

#    def testStorage(self):
#        pass

    #storeProposal(TargetList) #Stores proposal, returns pid.
    #getProposalStatus(pid) #Retrieves proposal status, returns status.
    #removeProposal(pid) #Removes proposal.
    #getProposalObservations(pid) #Returns images of observed targets. Raises ProposalNotYetReadyEx.
    #getProposals() #Returns observations waiting to be observed.
    #setProposalStatus(pid, status) #Sets the status of a given proposal. Raises InvalidProposalStatusTransitionEx.
    #storeImage(pid, tid, image) #Stores image for a given <pid,tid>. Raises ImageAlreadyStoredEx.
    #clean() #Cleans the database.
    def testDatabase(self):
        #Test getting proposal status of an incorrect id.
	self.assertEqual(self.database.getProposalStatus(0),-1) #Not sure the expected behavior when the pid doesn't exist.
        props = self.database.getProposals()
        self.assertEqual(len(props),0)
        #Test storing proposal.
        pid = self.database.storeProposal(self.getTargets())
	self.assertNotEqual(self.database.getProposalStatus(pid),1)
        props = self.database.getProposals()
        self.assertEqual(len(props),1)
        self.assertEqual(props[0].pid,pid)
        #Test remove proposal.
        self.removeProposal(pid)
	self.assertEqual(self.database.getProposalStatus(0),-1) #Not sure the expected behavior when the pid doesn't exist.
        #Test store image.
        targets = self.getTargets()
        pid = self.database.storeProposal(targets)
        img0 = [0]
        img1 = [1]
        self.database.setProposalStatus(pid, 1)
        self.database.storeImage(pid, targets[0].tid, img)
        self.database.storeImage(pid, targets[1].tid, img)
        self.database.setProposalStatus(pid, 2)
        imgs = self.database.getProposalObservations(pid)
        self.assertEqual(imgs[0], img0)
        self.assertEqual(imgs[1], img1)
        props = self.database.getProposals()
        self.assertEqual(len(props),0)
        #Test clean
        self.database.clean()


    #observe(coords, expTime) #Executes an observation, returns image. Raises PositionOutOfLimitsEx.
    #moveTo(coords) #Moves the telescope. Raises PositionOutOfLimitsEx.
    #getCurrentPosition() #Retrieves current position, returns coords.
    def testTelescope(self):
        self.assertEquals(self.getCurrentPosition().az,0) #Not sure of initial values?
        self.assertEquals(self.getCurrentPosition().el,0) #Not sure of initial values?
        pos = Position()
        pos.az = 100
        pos.el = 60
        self.moveTo(pos)
        time.sleep(10)
        self.assertEquals(self.getCurrentPosition().az,100)
        self.assertEquals(self.getCurrentPosition().el,60)
        img = observe(pos, 3)
        self.assertIsNotNone(imgs)
        pos.az = 100
        pos.el = 100
        self.assertRaises(PositionOutOfLimitsEx, moveTo, pos)
        self.assertRaises(PositionOutOfLimitsEx, observe, pos, 3)
        self.assertEquals(self.getCurrentPosition().az,100)
        self.assertEquals(self.getCurrentPosition().el,60)
        pos.az = 400
        pos.el = 60
        self.assertRaises(PositionOutOfLimitsEx, moveTo, pos)
        self.assertRaises(PositionOutOfLimitsEx, observe, pos, 3)
        self.assertEquals(self.getCurrentPosition().az,100)
        self.assertEquals(self.getCurrentPosition().el,60)

    #start() #The scheduler will start looping trying to observe. Raises SchedulerAlreadyRunningEx.
    #stop() #The scheduler will stop looping (as soon as the observation in action finishes). Raises SchedulerAlreadyStoppedEx.
    #proposalUnderExecution() #Returns the pid of the proposal under execution. Raises NoProposalExecutingEx.
    def testScheduler(self):
        #Test proposalUnderExecution, start and stop
	self.assertRaises(NoProposalExecutingEx, self.scheduler.proposalUnderExecution)
        self.scheduler.start()
	self.assertRaises(SchedulerAlreadyRunningEx, self.scheduler.start)
        self.scheduler.stop()
	self.assertRaises(SchedulerAlreadyStoppedEx, self.scheduler.stop)
        pid = self.database.storeProposal(self.getTargets())
        self.scheduler.start()
        time.sleep(2)
	self.assertEqual(self.scheduler.proposalUnderExecution(),pid)
        obsFinished = False
        while not obsFinished:
            try:
               pid = self.scheduler.proposalUnderExecution()
               obsFinished = True
            except NoProposalExecutingEx as e:
               time.sleep(1)
        self.scheduler.stop()
        imgs = self.database.getProposalObservations(pid)
        self.assertIsNotNone(imgs)
	self.assertEqual(self.database.getProposalStatus(pid),3)
        self.database.clean()

    #cameraOn() #Turns camera on 
    #cameraOff() #Turns camera off
    #takeImage(expTime) #Takes an image with expTime. Raises CameraIsOffEx.
    #setRGB(rgb) #Sets RGB config. Raises CameraIsOffEx.
    #setPixelBias(bias) #Sets Pixel Bias. Raises CameraIsOffEx.
    #setResetLevel(reset) #Sets Reset Level. Raises CameraIsOffEx.
    def testInstrument(self):
        self.assertRaises(CameraIsOffEx, self.instrument.takeImage, 1)
        self.assertRaises(CameraIsOffEx, self.instrument.setRGB, RGB())
        self.assertRaises(CameraIsOffEx, self.instrument.setPixelBias, 0)
        self.assertRaises(CameraIsOffEx, self.instrument.setResetLevel, 0)
        self.instrument.cameraOn()
        rgb = RGB()
        rgb.red = 10
        rgb.blue = 10
        rgb.green = 10
        self.setRGB(rgb)
        self.setPixelBias(10)
        self.setResetLevel(0)
        self.instrument.cameraOff()
        self.assertRaises(CameraIsOffEx, self.instrument.takeImage, 1)
        self.assertRaises(CameraIsOffEx, self.instrument.setRGB, RGB())
        self.assertRaises(CameraIsOffEx, self.instrument.setPixelBias, 0)
        self.assertRaises(CameraIsOffEx, self.instrument.setResetLevel, 0)
        self.instrument.cameraOn()
        img = takeImage(3)
        self.instrument.cameraOff()


#    def testConsole(self):
#        pass

if __name__ == "__main__":
    unittest.main()
