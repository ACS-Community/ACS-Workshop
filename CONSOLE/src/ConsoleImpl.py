from CONSOLE_MODULE__POA import Console
from TYPES import *
import SORTErrImpl
from Acspy.Servants.ContainerServices  import ContainerServices
from Acspy.Servants.ComponentLifecycle import ComponentLifecycle
from Acspy.Servants.ACSComponent       import ACSComponent


class ConsoleImpl (Console,  #CORBA stubs for IDL interface
                   ACSComponent,  #Base IDL interface
                   ContainerServices,  #Developer niceties
                   ComponentLifecycle):  #HLA stuff

    def __init__ (self):
        ACSComponent.__init__(self)
        ContainerServices.__init__(self)
        self.getLogger().logInfo("Initializing...") 
        self.mode = False


    def setMode (self, inmode):
        sch = self.getComponent("SCHEDULER")
        if inmode:
            try:
                 self.getLogger().logInfo("Starting Scheduler")
                 sch.start()
                 self.mode = inmode
            except SORTErrImpl.SchedulerAlreadyRunningExImpl:
                 self.getLogger().logErr("AlreadyInAutomaticEx")
                 raise SORTErrImpl.AlreadyInAutomaticExImpl()
        else:
             try:
                 self.getLogger().logInfo("Stopping Scheduler")
                 sch.stop()
                 self.mode = inmode
             except SORTErrImpl.SchedulerAlreadyStoppedExImpl:
                 raise SORTErrImpl.AlreadyInAutomaticExImpl()


    def getMode (self):
        return self.mode


    def cameraOn (self):
        if self.mode:
            raise SORTErrImpl.SystemInAutoModeExImpl()

        self.getLogger().logInfo("Turning camera on") 
        inst = self.getComponent("INSTRUMENT")
        inst.cameraOn()


    def cameraOff (self):
        if self.mode:
            raise SORTErrImpl.SystemInAutoModeExImpl()

        self.getLogger().logInfo("Turning camera off") 
        inst = self.getComponent("INSTRUMENT")
        inst.cameraOff()


    def setRGB (self, rgbConfig):
        if self.mode:
            raise SORTErrImpl.SystemInAutoModeExImpl()

        self.getLogger().logInfo("Setting camera RGB")
        inst = self.getComponent("INSTRUMENT")
        inst.setRGB(rgbConfig)


    def getCameraImage (self):
        if self.mode:
            raise SORTErrImpl.SystemInAutoModeExImpl()

        self.getLogger().logInfo("Taking camera image") 
        inst = self.getComponent("INSTRUMENT")
        img = inst.takeImage(3)

        return img


    def setPixelBias (self, bias):
        if self.mode:
            raise SORTErrImpl.SystemInAutoModeExImpl()

        self.getLogger().logInfo("Setting camera PixelBias") 
        inst = self.getComponent("INSTRUMENT")
        inst.setPixelBias(bias)


    def setResetLevel (self, resetLevel):
        if self.mode:
            raise SORTErrImpl.SystemInAutoModeExImpl()

        self.getLogger().logInfo("Setting camera reset level")
        inst = self.getComponent("INSTRUMENT")
        inst.setResetLevel(resetLevel)


    def moveTelescope (self, position):
        if self.mode:
            raise SORTErrImpl.SystemInAutoModeExImpl()

        self.getLogger().logInfo("Commanding telescope position")
        inst = self.getComponent("TELESCOPE")
        inst.moveTo(position)


    def getTelescopePosition (self):
        if self.mode:
            raise SORTErrImpl.SystemInAutoModeExImpl()

        self.getLogger().logInfo("Getting telescope position")
        inst = self.getComponent("TELESCOPE")
        pos = inst.getCurrentPosition()

        return pos

