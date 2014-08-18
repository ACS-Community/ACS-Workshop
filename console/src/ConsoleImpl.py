from CONSOLE_MODULE__POA import Console
from TYPES import *
import SORTErrImpl
from Acspy.Servants.ContainerServices  import ContainerServices
from Acspy.Servants.ComponentLifecycle import ComponentLifecycle
from Acspy.Servants.ACSComponent       import ACSComponent


class ConsoleImpl(Console,  #CORBA stubs for IDL interface
                ACSComponent,  #Base IDL interface
                ContainerServices,  #Developer niceties
                ComponentLifecycle):  #HLA stuff

    mode = False

    def __init__(self): 
        ACSComponent.__init__(self)
        ContainerServices.__init__(self)
        self.getLogger().logInfo("Initialization...") 
        
        return

    def setMode(self,inmode):
             
    	sch = self.getComponent("SCHEDULER")
    	self.getLogger().logInfo("getting SCHEDULER")
    	     
        if inmode:
            try:                
                 sch.start()
                 self.mode = inmode
            except SORTErrImpl.SchedulerAlreadyRunningExImpl:
                 self.getLogger().logErr("AlreadyInAutomaticEx")
                 raise SORTErrImpl.AlreadyInAutomaticExImpl()
          
        else:
             try:             
      #       	 self.getLogger().logInfo("stopping")
                 sch.stop()
                 self.mode = inmode
             except SORTErrImpl.SchedulerAlreadyStoppedExImpl:
                 raise SORTErrImpl.AlreadyInAutomaticExImpl()
        

    def getMode(self):
#    #	self.getLogger().logInfo("mode is: " + self.mode)
        return self.mode
	

    def cameraOn(self):	
        if self.mode:
            raise SORTErrImpl.SystemInAutoModeExImpl()

        inst = self.getComponent("INSTRUMENT")
        inst.cameraOn()
     #   self.getLogger().logInfo("cameraOn() called...") 

    def cameraOff(self):	
        if self.mode:
            raise SORTErrImpl.SystemInAutoModeExImpl()
                
        inst = self.getComponent("INSTRUMENT")
        inst.cameraOff()
       # self.getLogger().logInfo("cameraOff() called...") 

	
    def setRGB(self,rgbConfig):
        if self.mode:
            raise SORTErrImpl.SystemInAutoModeExImpl()
                
        inst = self.getComponent("INSTRUMENT")
        inst.setRGB(rgbConfig)
       # self.getLogger().logInfo("setRGB() called...")

    def getCameraImage(self):     
        if self.mode:
            raise SORTErrImpl.SystemInAutoModeExImpl()
            
        inst = self.getComponent("INSTRUMENT")	
        img = inst.takeImage(3)
        
       # self.getLogger().logInfo("getCameraImage() called...") 	 
        return img

    def setPixelBias(self,bias):
        if self.mode:
            raise SORTErrImpl.SystemInAutoModeExImpl()
                
        inst = self.getComponent("INSTRUMENT")
        inst.setPixelBias(bias)
       # self.getLogger().logInfo("setPixelBias() called...") 	 


    def setResetLevel(self,resetLevel):
        if self.mode:
            raise SORTErrImpl.SystemInAutoModeExImpl()
                
        inst = self.getComponent("INSTRUMENT")
        inst.setResetLevel(resetLevel)
       # self.getLogger().logInfo("setResetLevel() called...") 	
        
    def moveTelescope(self,position):
        if self.mode:
            raise SORTErrImpl.SystemInAutoModeExImpl()
            
    	inst = self.getComponent("TELESCOPE")
        inst.moveTo(position)
    
    def getTelescopePosition(self):
        if self.mode:
            raise SORTErrImpl.SystemInAutoModeExImpl()
            
    	inst = self.getComponent("TELESCOPE")
        pos = inst.getCurrentPosition()
        return pos    