#!/usr/bin/env python
# @(#) $Id: AES2Py.xslt,v 1.22 2011/03/24 16:53:35 tstaig Exp $
#
#    ALMA - Atacama Large Millimiter Array
#    (c) Associated Universities, Inc. Washington DC, USA,  2001
#    (c) European Southern Observatory, 2002
#    Copyright by ESO (in the framework of the ALMA collaboration)
#    and Cosylab 2002, All rights reserved
#
#    This library is free software; you can redistribute it and/or
#    modify it under the terms of the GNU Lesser General Public
#    License as published by the Free Software Foundation; either
#    version 2.1 of the License, or (at your option) any later version.
#
#    This library is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#    Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public
#    License along with this library; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
######################################################################
'''
Some form of custom documentation goes here...
'''
######################################################################
from   Acspy.Common.Err import ACSError
import ACSErr
import SYSTEMErr
from Acspy.Common.TimeHelper import getTimeStamp
######################################################################

class BaseException:
    '''
    Class serves as a base exception for all error type/code exception
    pairs defined within this module. The reason this is provided is so 
    that one can generically catch ACS Error System based Python 
    exceptions using a single Python "except BaseException, e:" type
    statement.
    '''
    pass
######################################################################
class AlreadyInAutomaticExImpl(SYSTEMErr.AlreadyInAutomaticEx, ACSError, BaseException):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from the CORBA class of 
        similar name. The difference between the two is that this class
        provides many additional helper methods from Acspy.Common.Err.

        There are three different combinations of keyword parameter
        uses that make sense here:

            __init__()
              Using the default values creates a new exception which 
              does not include any previous error traces
            
            __init__(exception=someOldException)
              Specifying a previous ACS Error System exception or 
              without changing the value of create 
              creates a new exception which does in fact include 
              previous error traces from someOldException.

            __init__(exception=someOldException, create=0)
              Used to reconstruct someOldException without adding any
              new error trace information. It is absolutely critical
              that someOldException be of the same CORBA type as this
              class implements!

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              exception is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the exception. Only used when
        create has a value of 1/True
        - exception is an ACS Error System based CORBA exception
        Provide this to extract previous error trace 
        information and put this into the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this exception
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out what went wrong
        most likely you want create to have a value of 0. However, if you
        intend on rethrowing the exception a value of 1 makes more sense.
        - severity is used to set the severity of exception. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "Already in automatic mode"
        description = self.shortDescription
        ACSError.__init__(self,  
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.AlreadyInAutomatic,
                          exception,
                          description,
                          nvSeq,                          
                          create,
                          severity)
        SYSTEMErr.AlreadyInAutomaticEx.__init__(self, self.errorTrace)
        return
    #--------------------------------------------------------------------------        
    def getSYSTEMErrEx(self):
        '''
        Returns this exception converted into an SYSTEMErrEx
        '''
        return SYSTEMErr.SYSTEMErrEx(self.getErrorTrace())


######################################################################
class PositionOutOfLimitsExImpl(SYSTEMErr.PositionOutOfLimitsEx, ACSError, BaseException):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from the CORBA class of 
        similar name. The difference between the two is that this class
        provides many additional helper methods from Acspy.Common.Err.

        There are three different combinations of keyword parameter
        uses that make sense here:

            __init__()
              Using the default values creates a new exception which 
              does not include any previous error traces
            
            __init__(exception=someOldException)
              Specifying a previous ACS Error System exception or 
              without changing the value of create 
              creates a new exception which does in fact include 
              previous error traces from someOldException.

            __init__(exception=someOldException, create=0)
              Used to reconstruct someOldException without adding any
              new error trace information. It is absolutely critical
              that someOldException be of the same CORBA type as this
              class implements!

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              exception is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the exception. Only used when
        create has a value of 1/True
        - exception is an ACS Error System based CORBA exception
        Provide this to extract previous error trace 
        information and put this into the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this exception
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out what went wrong
        most likely you want create to have a value of 0. However, if you
        intend on rethrowing the exception a value of 1 makes more sense.
        - severity is used to set the severity of exception. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "Position out of limits"
        description = self.shortDescription
        ACSError.__init__(self,  
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.PositionOutOfLimits,
                          exception,
                          description,
                          nvSeq,                          
                          create,
                          severity)
        SYSTEMErr.PositionOutOfLimitsEx.__init__(self, self.errorTrace)
        return
    #--------------------------------------------------------------------------        
    def getSYSTEMErrEx(self):
        '''
        Returns this exception converted into an SYSTEMErrEx
        '''
        return SYSTEMErr.SYSTEMErrEx(self.getErrorTrace())


######################################################################
class ProposalNotYetReadyExImpl(SYSTEMErr.ProposalNotYetReadyEx, ACSError, BaseException):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from the CORBA class of 
        similar name. The difference between the two is that this class
        provides many additional helper methods from Acspy.Common.Err.

        There are three different combinations of keyword parameter
        uses that make sense here:

            __init__()
              Using the default values creates a new exception which 
              does not include any previous error traces
            
            __init__(exception=someOldException)
              Specifying a previous ACS Error System exception or 
              without changing the value of create 
              creates a new exception which does in fact include 
              previous error traces from someOldException.

            __init__(exception=someOldException, create=0)
              Used to reconstruct someOldException without adding any
              new error trace information. It is absolutely critical
              that someOldException be of the same CORBA type as this
              class implements!

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              exception is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the exception. Only used when
        create has a value of 1/True
        - exception is an ACS Error System based CORBA exception
        Provide this to extract previous error trace 
        information and put this into the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this exception
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out what went wrong
        most likely you want create to have a value of 0. However, if you
        intend on rethrowing the exception a value of 1 makes more sense.
        - severity is used to set the severity of exception. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "Proposal is not ready"
        description = self.shortDescription
        ACSError.__init__(self,  
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.ProposalNotYetReady,
                          exception,
                          description,
                          nvSeq,                          
                          create,
                          severity)
        SYSTEMErr.ProposalNotYetReadyEx.__init__(self, self.errorTrace)
        return
    #--------------------------------------------------------------------------        
    def getSYSTEMErrEx(self):
        '''
        Returns this exception converted into an SYSTEMErrEx
        '''
        return SYSTEMErr.SYSTEMErrEx(self.getErrorTrace())


######################################################################
class InvalidProposalStatusTransitionExImpl(SYSTEMErr.InvalidProposalStatusTransitionEx, ACSError, BaseException):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from the CORBA class of 
        similar name. The difference between the two is that this class
        provides many additional helper methods from Acspy.Common.Err.

        There are three different combinations of keyword parameter
        uses that make sense here:

            __init__()
              Using the default values creates a new exception which 
              does not include any previous error traces
            
            __init__(exception=someOldException)
              Specifying a previous ACS Error System exception or 
              without changing the value of create 
              creates a new exception which does in fact include 
              previous error traces from someOldException.

            __init__(exception=someOldException, create=0)
              Used to reconstruct someOldException without adding any
              new error trace information. It is absolutely critical
              that someOldException be of the same CORBA type as this
              class implements!

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              exception is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the exception. Only used when
        create has a value of 1/True
        - exception is an ACS Error System based CORBA exception
        Provide this to extract previous error trace 
        information and put this into the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this exception
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out what went wrong
        most likely you want create to have a value of 0. However, if you
        intend on rethrowing the exception a value of 1 makes more sense.
        - severity is used to set the severity of exception. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "Database: Invalid proposal status"
        description = self.shortDescription
        ACSError.__init__(self,  
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.InvalidProposalStatusTransition,
                          exception,
                          description,
                          nvSeq,                          
                          create,
                          severity)
        SYSTEMErr.InvalidProposalStatusTransitionEx.__init__(self, self.errorTrace)
        return
    #--------------------------------------------------------------------------        
    def getSYSTEMErrEx(self):
        '''
        Returns this exception converted into an SYSTEMErrEx
        '''
        return SYSTEMErr.SYSTEMErrEx(self.getErrorTrace())


######################################################################
class ImageAlreadyStoredExImpl(SYSTEMErr.ImageAlreadyStoredEx, ACSError, BaseException):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from the CORBA class of 
        similar name. The difference between the two is that this class
        provides many additional helper methods from Acspy.Common.Err.

        There are three different combinations of keyword parameter
        uses that make sense here:

            __init__()
              Using the default values creates a new exception which 
              does not include any previous error traces
            
            __init__(exception=someOldException)
              Specifying a previous ACS Error System exception or 
              without changing the value of create 
              creates a new exception which does in fact include 
              previous error traces from someOldException.

            __init__(exception=someOldException, create=0)
              Used to reconstruct someOldException without adding any
              new error trace information. It is absolutely critical
              that someOldException be of the same CORBA type as this
              class implements!

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              exception is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the exception. Only used when
        create has a value of 1/True
        - exception is an ACS Error System based CORBA exception
        Provide this to extract previous error trace 
        information and put this into the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this exception
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out what went wrong
        most likely you want create to have a value of 0. However, if you
        intend on rethrowing the exception a value of 1 makes more sense.
        - severity is used to set the severity of exception. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "Image already stored"
        description = self.shortDescription
        ACSError.__init__(self,  
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.ImageAlreadyStored,
                          exception,
                          description,
                          nvSeq,                          
                          create,
                          severity)
        SYSTEMErr.ImageAlreadyStoredEx.__init__(self, self.errorTrace)
        return
    #--------------------------------------------------------------------------        
    def getSYSTEMErrEx(self):
        '''
        Returns this exception converted into an SYSTEMErrEx
        '''
        return SYSTEMErr.SYSTEMErrEx(self.getErrorTrace())


######################################################################
class CameraIsOffExImpl(SYSTEMErr.CameraIsOffEx, ACSError, BaseException):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from the CORBA class of 
        similar name. The difference between the two is that this class
        provides many additional helper methods from Acspy.Common.Err.

        There are three different combinations of keyword parameter
        uses that make sense here:

            __init__()
              Using the default values creates a new exception which 
              does not include any previous error traces
            
            __init__(exception=someOldException)
              Specifying a previous ACS Error System exception or 
              without changing the value of create 
              creates a new exception which does in fact include 
              previous error traces from someOldException.

            __init__(exception=someOldException, create=0)
              Used to reconstruct someOldException without adding any
              new error trace information. It is absolutely critical
              that someOldException be of the same CORBA type as this
              class implements!

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              exception is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the exception. Only used when
        create has a value of 1/True
        - exception is an ACS Error System based CORBA exception
        Provide this to extract previous error trace 
        information and put this into the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this exception
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out what went wrong
        most likely you want create to have a value of 0. However, if you
        intend on rethrowing the exception a value of 1 makes more sense.
        - severity is used to set the severity of exception. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "camera is off"
        description = self.shortDescription
        ACSError.__init__(self,  
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.CameraIsOff,
                          exception,
                          description,
                          nvSeq,                          
                          create,
                          severity)
        SYSTEMErr.CameraIsOffEx.__init__(self, self.errorTrace)
        return
    #--------------------------------------------------------------------------        
    def getSYSTEMErrEx(self):
        '''
        Returns this exception converted into an SYSTEMErrEx
        '''
        return SYSTEMErr.SYSTEMErrEx(self.getErrorTrace())


######################################################################
class SchedulerAlreadyRunningExImpl(SYSTEMErr.SchedulerAlreadyRunningEx, ACSError, BaseException):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from the CORBA class of 
        similar name. The difference between the two is that this class
        provides many additional helper methods from Acspy.Common.Err.

        There are three different combinations of keyword parameter
        uses that make sense here:

            __init__()
              Using the default values creates a new exception which 
              does not include any previous error traces
            
            __init__(exception=someOldException)
              Specifying a previous ACS Error System exception or 
              without changing the value of create 
              creates a new exception which does in fact include 
              previous error traces from someOldException.

            __init__(exception=someOldException, create=0)
              Used to reconstruct someOldException without adding any
              new error trace information. It is absolutely critical
              that someOldException be of the same CORBA type as this
              class implements!

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              exception is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the exception. Only used when
        create has a value of 1/True
        - exception is an ACS Error System based CORBA exception
        Provide this to extract previous error trace 
        information and put this into the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this exception
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out what went wrong
        most likely you want create to have a value of 0. However, if you
        intend on rethrowing the exception a value of 1 makes more sense.
        - severity is used to set the severity of exception. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "Scheduler is already running"
        description = self.shortDescription
        ACSError.__init__(self,  
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.SchedulerAlreadyRunning,
                          exception,
                          description,
                          nvSeq,                          
                          create,
                          severity)
        SYSTEMErr.SchedulerAlreadyRunningEx.__init__(self, self.errorTrace)
        return
    #--------------------------------------------------------------------------        
    def getSYSTEMErrEx(self):
        '''
        Returns this exception converted into an SYSTEMErrEx
        '''
        return SYSTEMErr.SYSTEMErrEx(self.getErrorTrace())


######################################################################
class SchedulerAlreadyStoppedExImpl(SYSTEMErr.SchedulerAlreadyStoppedEx, ACSError, BaseException):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from the CORBA class of 
        similar name. The difference between the two is that this class
        provides many additional helper methods from Acspy.Common.Err.

        There are three different combinations of keyword parameter
        uses that make sense here:

            __init__()
              Using the default values creates a new exception which 
              does not include any previous error traces
            
            __init__(exception=someOldException)
              Specifying a previous ACS Error System exception or 
              without changing the value of create 
              creates a new exception which does in fact include 
              previous error traces from someOldException.

            __init__(exception=someOldException, create=0)
              Used to reconstruct someOldException without adding any
              new error trace information. It is absolutely critical
              that someOldException be of the same CORBA type as this
              class implements!

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              exception is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the exception. Only used when
        create has a value of 1/True
        - exception is an ACS Error System based CORBA exception
        Provide this to extract previous error trace 
        information and put this into the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this exception
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out what went wrong
        most likely you want create to have a value of 0. However, if you
        intend on rethrowing the exception a value of 1 makes more sense.
        - severity is used to set the severity of exception. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "Scheduler is already stopped"
        description = self.shortDescription
        ACSError.__init__(self,  
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.SchedulerAlreadyStopped,
                          exception,
                          description,
                          nvSeq,                          
                          create,
                          severity)
        SYSTEMErr.SchedulerAlreadyStoppedEx.__init__(self, self.errorTrace)
        return
    #--------------------------------------------------------------------------        
    def getSYSTEMErrEx(self):
        '''
        Returns this exception converted into an SYSTEMErrEx
        '''
        return SYSTEMErr.SYSTEMErrEx(self.getErrorTrace())


######################################################################
class NoProposalExecutingExImpl(SYSTEMErr.NoProposalExecutingEx, ACSError, BaseException):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from the CORBA class of 
        similar name. The difference between the two is that this class
        provides many additional helper methods from Acspy.Common.Err.

        There are three different combinations of keyword parameter
        uses that make sense here:

            __init__()
              Using the default values creates a new exception which 
              does not include any previous error traces
            
            __init__(exception=someOldException)
              Specifying a previous ACS Error System exception or 
              without changing the value of create 
              creates a new exception which does in fact include 
              previous error traces from someOldException.

            __init__(exception=someOldException, create=0)
              Used to reconstruct someOldException without adding any
              new error trace information. It is absolutely critical
              that someOldException be of the same CORBA type as this
              class implements!

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              exception is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the exception. Only used when
        create has a value of 1/True
        - exception is an ACS Error System based CORBA exception
        Provide this to extract previous error trace 
        information and put this into the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this exception
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out what went wrong
        most likely you want create to have a value of 0. However, if you
        intend on rethrowing the exception a value of 1 makes more sense.
        - severity is used to set the severity of exception. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "No proposal is executing"
        description = self.shortDescription
        ACSError.__init__(self,  
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.NoProposalExecuting,
                          exception,
                          description,
                          nvSeq,                          
                          create,
                          severity)
        SYSTEMErr.NoProposalExecutingEx.__init__(self, self.errorTrace)
        return
    #--------------------------------------------------------------------------        
    def getSYSTEMErrEx(self):
        '''
        Returns this exception converted into an SYSTEMErrEx
        '''
        return SYSTEMErr.SYSTEMErrEx(self.getErrorTrace())


######################################################################
class SystemInAutoModeExImpl(SYSTEMErr.SystemInAutoModeEx, ACSError, BaseException):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from the CORBA class of 
        similar name. The difference between the two is that this class
        provides many additional helper methods from Acspy.Common.Err.

        There are three different combinations of keyword parameter
        uses that make sense here:

            __init__()
              Using the default values creates a new exception which 
              does not include any previous error traces
            
            __init__(exception=someOldException)
              Specifying a previous ACS Error System exception or 
              without changing the value of create 
              creates a new exception which does in fact include 
              previous error traces from someOldException.

            __init__(exception=someOldException, create=0)
              Used to reconstruct someOldException without adding any
              new error trace information. It is absolutely critical
              that someOldException be of the same CORBA type as this
              class implements!

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              exception is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the exception. Only used when
        create has a value of 1/True
        - exception is an ACS Error System based CORBA exception
        Provide this to extract previous error trace 
        information and put this into the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this exception
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out what went wrong
        most likely you want create to have a value of 0. However, if you
        intend on rethrowing the exception a value of 1 makes more sense.
        - severity is used to set the severity of exception. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "System is in automatic mode"
        description = self.shortDescription
        ACSError.__init__(self,  
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.SystemInAutoMode,
                          exception,
                          description,
                          nvSeq,                          
                          create,
                          severity)
        SYSTEMErr.SystemInAutoModeEx.__init__(self, self.errorTrace)
        return
    #--------------------------------------------------------------------------        
    def getSYSTEMErrEx(self):
        '''
        Returns this exception converted into an SYSTEMErrEx
        '''
        return SYSTEMErr.SYSTEMErrEx(self.getErrorTrace())


######################################################################
class CannotOpenDeviceExImpl(SYSTEMErr.CannotOpenDeviceEx, ACSError, BaseException):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from the CORBA class of 
        similar name. The difference between the two is that this class
        provides many additional helper methods from Acspy.Common.Err.

        There are three different combinations of keyword parameter
        uses that make sense here:

            __init__()
              Using the default values creates a new exception which 
              does not include any previous error traces
            
            __init__(exception=someOldException)
              Specifying a previous ACS Error System exception or 
              without changing the value of create 
              creates a new exception which does in fact include 
              previous error traces from someOldException.

            __init__(exception=someOldException, create=0)
              Used to reconstruct someOldException without adding any
              new error trace information. It is absolutely critical
              that someOldException be of the same CORBA type as this
              class implements!

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              exception is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the exception. Only used when
        create has a value of 1/True
        - exception is an ACS Error System based CORBA exception
        Provide this to extract previous error trace 
        information and put this into the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this exception
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out what went wrong
        most likely you want create to have a value of 0. However, if you
        intend on rethrowing the exception a value of 1 makes more sense.
        - severity is used to set the severity of exception. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "Can't open device"
        description = self.shortDescription
        ACSError.__init__(self,  
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.CannotOpenDevice,
                          exception,
                          description,
                          nvSeq,                          
                          create,
                          severity)
        SYSTEMErr.CannotOpenDeviceEx.__init__(self, self.errorTrace)
        return
    #--------------------------------------------------------------------------        
    def getSYSTEMErrEx(self):
        '''
        Returns this exception converted into an SYSTEMErrEx
        '''
        return SYSTEMErr.SYSTEMErrEx(self.getErrorTrace())


######################################################################
class AlreadyInAutomaticCompletionImpl(ACSErr.Completion, ACSError):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from ACSErr.Completion. 
        It provides many helper methods from Acspy.Common.Err.
        
        There are three different combinations of keyword parameter
        uses that make sense here:
            
            __init__()
              Using the default values creates a new Completion which 
              does not include any previous error traces
            
            __init__(exception=acsException)
              Specifying a previous ACS Error System exception without
              changing the value of create creates a new Completion which
              does in fact include previous error traces from
              acsException.
            
            __init__(exception=acsException, create=0)
              Used to reconstruct acsException without adding any
              new error trace information.

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              Completion is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the Completion. Only used when
        create has a value of 1
        - exception is an ACS Error System based CORBA exception. 
	Provide this to extract previous error trace information and put this into
        the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this Completion
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out 
        what went wrong most likely you want create to have a value of 0. 
        However, if you intend on returning the Completion a value of 1 makes 
        more sense.
        - severity is used to set the severity of the completion. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "Already in automatic mode"
        description = self.shortDescription
        ACSError.__init__(self,
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.AlreadyInAutomatic,
                          exception,
                          description,
                          nvSeq,
                          create,
                          severity)

        #Create the CORBA object
        ACSErr.Completion.__init__(self,
                                   self.getTimeStamp(),
                                   self.getErrorType(),
                                   self.getErrorCode(),
                                   [self.errorTrace])
        return


######################################################################
class PositionOutOfLimitsCompletionImpl(ACSErr.Completion, ACSError):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from ACSErr.Completion. 
        It provides many helper methods from Acspy.Common.Err.
        
        There are three different combinations of keyword parameter
        uses that make sense here:
            
            __init__()
              Using the default values creates a new Completion which 
              does not include any previous error traces
            
            __init__(exception=acsException)
              Specifying a previous ACS Error System exception without
              changing the value of create creates a new Completion which
              does in fact include previous error traces from
              acsException.
            
            __init__(exception=acsException, create=0)
              Used to reconstruct acsException without adding any
              new error trace information.

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              Completion is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the Completion. Only used when
        create has a value of 1
        - exception is an ACS Error System based CORBA exception. 
	Provide this to extract previous error trace information and put this into
        the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this Completion
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out 
        what went wrong most likely you want create to have a value of 0. 
        However, if you intend on returning the Completion a value of 1 makes 
        more sense.
        - severity is used to set the severity of the completion. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "Position out of limits"
        description = self.shortDescription
        ACSError.__init__(self,
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.PositionOutOfLimits,
                          exception,
                          description,
                          nvSeq,
                          create,
                          severity)

        #Create the CORBA object
        ACSErr.Completion.__init__(self,
                                   self.getTimeStamp(),
                                   self.getErrorType(),
                                   self.getErrorCode(),
                                   [self.errorTrace])
        return


######################################################################
class ProposalNotYetReadyCompletionImpl(ACSErr.Completion, ACSError):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from ACSErr.Completion. 
        It provides many helper methods from Acspy.Common.Err.
        
        There are three different combinations of keyword parameter
        uses that make sense here:
            
            __init__()
              Using the default values creates a new Completion which 
              does not include any previous error traces
            
            __init__(exception=acsException)
              Specifying a previous ACS Error System exception without
              changing the value of create creates a new Completion which
              does in fact include previous error traces from
              acsException.
            
            __init__(exception=acsException, create=0)
              Used to reconstruct acsException without adding any
              new error trace information.

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              Completion is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the Completion. Only used when
        create has a value of 1
        - exception is an ACS Error System based CORBA exception. 
	Provide this to extract previous error trace information and put this into
        the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this Completion
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out 
        what went wrong most likely you want create to have a value of 0. 
        However, if you intend on returning the Completion a value of 1 makes 
        more sense.
        - severity is used to set the severity of the completion. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "Proposal is not ready"
        description = self.shortDescription
        ACSError.__init__(self,
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.ProposalNotYetReady,
                          exception,
                          description,
                          nvSeq,
                          create,
                          severity)

        #Create the CORBA object
        ACSErr.Completion.__init__(self,
                                   self.getTimeStamp(),
                                   self.getErrorType(),
                                   self.getErrorCode(),
                                   [self.errorTrace])
        return


######################################################################
class InvalidProposalStatusTransitionCompletionImpl(ACSErr.Completion, ACSError):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from ACSErr.Completion. 
        It provides many helper methods from Acspy.Common.Err.
        
        There are three different combinations of keyword parameter
        uses that make sense here:
            
            __init__()
              Using the default values creates a new Completion which 
              does not include any previous error traces
            
            __init__(exception=acsException)
              Specifying a previous ACS Error System exception without
              changing the value of create creates a new Completion which
              does in fact include previous error traces from
              acsException.
            
            __init__(exception=acsException, create=0)
              Used to reconstruct acsException without adding any
              new error trace information.

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              Completion is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the Completion. Only used when
        create has a value of 1
        - exception is an ACS Error System based CORBA exception. 
	Provide this to extract previous error trace information and put this into
        the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this Completion
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out 
        what went wrong most likely you want create to have a value of 0. 
        However, if you intend on returning the Completion a value of 1 makes 
        more sense.
        - severity is used to set the severity of the completion. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "Database: Invalid proposal status"
        description = self.shortDescription
        ACSError.__init__(self,
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.InvalidProposalStatusTransition,
                          exception,
                          description,
                          nvSeq,
                          create,
                          severity)

        #Create the CORBA object
        ACSErr.Completion.__init__(self,
                                   self.getTimeStamp(),
                                   self.getErrorType(),
                                   self.getErrorCode(),
                                   [self.errorTrace])
        return


######################################################################
class ImageAlreadyStoredCompletionImpl(ACSErr.Completion, ACSError):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from ACSErr.Completion. 
        It provides many helper methods from Acspy.Common.Err.
        
        There are three different combinations of keyword parameter
        uses that make sense here:
            
            __init__()
              Using the default values creates a new Completion which 
              does not include any previous error traces
            
            __init__(exception=acsException)
              Specifying a previous ACS Error System exception without
              changing the value of create creates a new Completion which
              does in fact include previous error traces from
              acsException.
            
            __init__(exception=acsException, create=0)
              Used to reconstruct acsException without adding any
              new error trace information.

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              Completion is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the Completion. Only used when
        create has a value of 1
        - exception is an ACS Error System based CORBA exception. 
	Provide this to extract previous error trace information and put this into
        the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this Completion
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out 
        what went wrong most likely you want create to have a value of 0. 
        However, if you intend on returning the Completion a value of 1 makes 
        more sense.
        - severity is used to set the severity of the completion. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "Image already stored"
        description = self.shortDescription
        ACSError.__init__(self,
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.ImageAlreadyStored,
                          exception,
                          description,
                          nvSeq,
                          create,
                          severity)

        #Create the CORBA object
        ACSErr.Completion.__init__(self,
                                   self.getTimeStamp(),
                                   self.getErrorType(),
                                   self.getErrorCode(),
                                   [self.errorTrace])
        return


######################################################################
class CameraIsOffCompletionImpl(ACSErr.Completion, ACSError):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from ACSErr.Completion. 
        It provides many helper methods from Acspy.Common.Err.
        
        There are three different combinations of keyword parameter
        uses that make sense here:
            
            __init__()
              Using the default values creates a new Completion which 
              does not include any previous error traces
            
            __init__(exception=acsException)
              Specifying a previous ACS Error System exception without
              changing the value of create creates a new Completion which
              does in fact include previous error traces from
              acsException.
            
            __init__(exception=acsException, create=0)
              Used to reconstruct acsException without adding any
              new error trace information.

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              Completion is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the Completion. Only used when
        create has a value of 1
        - exception is an ACS Error System based CORBA exception. 
	Provide this to extract previous error trace information and put this into
        the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this Completion
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out 
        what went wrong most likely you want create to have a value of 0. 
        However, if you intend on returning the Completion a value of 1 makes 
        more sense.
        - severity is used to set the severity of the completion. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "camera is off"
        description = self.shortDescription
        ACSError.__init__(self,
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.CameraIsOff,
                          exception,
                          description,
                          nvSeq,
                          create,
                          severity)

        #Create the CORBA object
        ACSErr.Completion.__init__(self,
                                   self.getTimeStamp(),
                                   self.getErrorType(),
                                   self.getErrorCode(),
                                   [self.errorTrace])
        return


######################################################################
class SchedulerAlreadyRunningCompletionImpl(ACSErr.Completion, ACSError):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from ACSErr.Completion. 
        It provides many helper methods from Acspy.Common.Err.
        
        There are three different combinations of keyword parameter
        uses that make sense here:
            
            __init__()
              Using the default values creates a new Completion which 
              does not include any previous error traces
            
            __init__(exception=acsException)
              Specifying a previous ACS Error System exception without
              changing the value of create creates a new Completion which
              does in fact include previous error traces from
              acsException.
            
            __init__(exception=acsException, create=0)
              Used to reconstruct acsException without adding any
              new error trace information.

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              Completion is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the Completion. Only used when
        create has a value of 1
        - exception is an ACS Error System based CORBA exception. 
	Provide this to extract previous error trace information and put this into
        the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this Completion
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out 
        what went wrong most likely you want create to have a value of 0. 
        However, if you intend on returning the Completion a value of 1 makes 
        more sense.
        - severity is used to set the severity of the completion. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "Scheduler is already running"
        description = self.shortDescription
        ACSError.__init__(self,
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.SchedulerAlreadyRunning,
                          exception,
                          description,
                          nvSeq,
                          create,
                          severity)

        #Create the CORBA object
        ACSErr.Completion.__init__(self,
                                   self.getTimeStamp(),
                                   self.getErrorType(),
                                   self.getErrorCode(),
                                   [self.errorTrace])
        return


######################################################################
class SchedulerAlreadyStoppedCompletionImpl(ACSErr.Completion, ACSError):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from ACSErr.Completion. 
        It provides many helper methods from Acspy.Common.Err.
        
        There are three different combinations of keyword parameter
        uses that make sense here:
            
            __init__()
              Using the default values creates a new Completion which 
              does not include any previous error traces
            
            __init__(exception=acsException)
              Specifying a previous ACS Error System exception without
              changing the value of create creates a new Completion which
              does in fact include previous error traces from
              acsException.
            
            __init__(exception=acsException, create=0)
              Used to reconstruct acsException without adding any
              new error trace information.

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              Completion is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the Completion. Only used when
        create has a value of 1
        - exception is an ACS Error System based CORBA exception. 
	Provide this to extract previous error trace information and put this into
        the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this Completion
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out 
        what went wrong most likely you want create to have a value of 0. 
        However, if you intend on returning the Completion a value of 1 makes 
        more sense.
        - severity is used to set the severity of the completion. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "Scheduler is already stopped"
        description = self.shortDescription
        ACSError.__init__(self,
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.SchedulerAlreadyStopped,
                          exception,
                          description,
                          nvSeq,
                          create,
                          severity)

        #Create the CORBA object
        ACSErr.Completion.__init__(self,
                                   self.getTimeStamp(),
                                   self.getErrorType(),
                                   self.getErrorCode(),
                                   [self.errorTrace])
        return


######################################################################
class NoProposalExecutingCompletionImpl(ACSErr.Completion, ACSError):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from ACSErr.Completion. 
        It provides many helper methods from Acspy.Common.Err.
        
        There are three different combinations of keyword parameter
        uses that make sense here:
            
            __init__()
              Using the default values creates a new Completion which 
              does not include any previous error traces
            
            __init__(exception=acsException)
              Specifying a previous ACS Error System exception without
              changing the value of create creates a new Completion which
              does in fact include previous error traces from
              acsException.
            
            __init__(exception=acsException, create=0)
              Used to reconstruct acsException without adding any
              new error trace information.

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              Completion is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the Completion. Only used when
        create has a value of 1
        - exception is an ACS Error System based CORBA exception. 
	Provide this to extract previous error trace information and put this into
        the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this Completion
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out 
        what went wrong most likely you want create to have a value of 0. 
        However, if you intend on returning the Completion a value of 1 makes 
        more sense.
        - severity is used to set the severity of the completion. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "No proposal is executing"
        description = self.shortDescription
        ACSError.__init__(self,
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.NoProposalExecuting,
                          exception,
                          description,
                          nvSeq,
                          create,
                          severity)

        #Create the CORBA object
        ACSErr.Completion.__init__(self,
                                   self.getTimeStamp(),
                                   self.getErrorType(),
                                   self.getErrorCode(),
                                   [self.errorTrace])
        return


######################################################################
class SystemInAutoModeCompletionImpl(ACSErr.Completion, ACSError):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from ACSErr.Completion. 
        It provides many helper methods from Acspy.Common.Err.
        
        There are three different combinations of keyword parameter
        uses that make sense here:
            
            __init__()
              Using the default values creates a new Completion which 
              does not include any previous error traces
            
            __init__(exception=acsException)
              Specifying a previous ACS Error System exception without
              changing the value of create creates a new Completion which
              does in fact include previous error traces from
              acsException.
            
            __init__(exception=acsException, create=0)
              Used to reconstruct acsException without adding any
              new error trace information.

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              Completion is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the Completion. Only used when
        create has a value of 1
        - exception is an ACS Error System based CORBA exception. 
	Provide this to extract previous error trace information and put this into
        the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this Completion
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out 
        what went wrong most likely you want create to have a value of 0. 
        However, if you intend on returning the Completion a value of 1 makes 
        more sense.
        - severity is used to set the severity of the completion. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "System is in automatic mode"
        description = self.shortDescription
        ACSError.__init__(self,
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.SystemInAutoMode,
                          exception,
                          description,
                          nvSeq,
                          create,
                          severity)

        #Create the CORBA object
        ACSErr.Completion.__init__(self,
                                   self.getTimeStamp(),
                                   self.getErrorType(),
                                   self.getErrorCode(),
                                   [self.errorTrace])
        return


######################################################################
class CannotOpenDeviceCompletionImpl(ACSErr.Completion, ACSError):
    '''
    Some form of custom documentation goes here...
    '''
    #-----------------------------------------------------------------
    def __init__(self,
                 nvSeq = None,
                 exception = None,
                 create = 1,
                 severity = None):
        '''
        Constructor
        
        An instance of this class is derived from ACSErr.Completion. 
        It provides many helper methods from Acspy.Common.Err.
        
        There are three different combinations of keyword parameter
        uses that make sense here:
            
            __init__()
              Using the default values creates a new Completion which 
              does not include any previous error traces
            
            __init__(exception=acsException)
              Specifying a previous ACS Error System exception without
              changing the value of create creates a new Completion which
              does in fact include previous error traces from
              acsException.
            
            __init__(exception=acsException, create=0)
              Used to reconstruct acsException without adding any
              new error trace information.

            nvSeq default keyword parameter
              This sequence of name/values is only used when a new 
              Completion is being created. In simple terms, the only
              time you can use it is when the create keyword parameter
              has the value of 1

            severity default keyword parameter
              This CORBA type corresponds to ACSErr.Severity. The
              only time you can use it is when the create keyword parameter
              has the value of 1

        Parameters:
        - nvSeq is a sequence of ACSErr.NameValue pairs used to add
        additional information about the Completion. Only used when
        create has a value of 1
        - exception is an ACS Error System based CORBA exception. 
	Provide this to extract previous error trace information and put this into
        the new object being constructed
        - create is a boolean value which defines whether or not traceback
        information should be extracted from the call to create this Completion
        and added to it's error trace. If you're simply trying to recreate
        a remote CORBA exception locally and figure out 
        what went wrong most likely you want create to have a value of 0. 
        However, if you intend on returning the Completion a value of 1 makes 
        more sense.
        - severity is used to set the severity of the completion. Only used when
        create has a value of 1/True
        '''
        if nvSeq == None:
            nvSeq = []
        self.shortDescription = "Can't open device"
        description = self.shortDescription
        ACSError.__init__(self,
                          ACSErr.SYSTEMErr,
                          SYSTEMErr.CannotOpenDevice,
                          exception,
                          description,
                          nvSeq,
                          create,
                          severity)

        #Create the CORBA object
        ACSErr.Completion.__init__(self,
                                   self.getTimeStamp(),
                                   self.getErrorType(),
                                   self.getErrorCode(),
                                   [self.errorTrace])
        return


######################################################################
if __name__ == "__main__":

    try:
        raise AlreadyInAutomaticExImpl
    except SYSTEMErr.AlreadyInAutomaticEx, e:
        print "Caught the correct type of exception:", e
        g = AlreadyInAutomaticExImpl(exception=e)
        g.Print()
    except Exception, e:
        print "Caught the wrong type of exception:", e

    try:
        raise PositionOutOfLimitsExImpl
    except SYSTEMErr.PositionOutOfLimitsEx, e:
        print "Caught the correct type of exception:", e
        g = PositionOutOfLimitsExImpl(exception=e)
        g.Print()
    except Exception, e:
        print "Caught the wrong type of exception:", e

    try:
        raise ProposalNotYetReadyExImpl
    except SYSTEMErr.ProposalNotYetReadyEx, e:
        print "Caught the correct type of exception:", e
        g = ProposalNotYetReadyExImpl(exception=e)
        g.Print()
    except Exception, e:
        print "Caught the wrong type of exception:", e

    try:
        raise InvalidProposalStatusTransitionExImpl
    except SYSTEMErr.InvalidProposalStatusTransitionEx, e:
        print "Caught the correct type of exception:", e
        g = InvalidProposalStatusTransitionExImpl(exception=e)
        g.Print()
    except Exception, e:
        print "Caught the wrong type of exception:", e

    try:
        raise ImageAlreadyStoredExImpl
    except SYSTEMErr.ImageAlreadyStoredEx, e:
        print "Caught the correct type of exception:", e
        g = ImageAlreadyStoredExImpl(exception=e)
        g.Print()
    except Exception, e:
        print "Caught the wrong type of exception:", e

    try:
        raise CameraIsOffExImpl
    except SYSTEMErr.CameraIsOffEx, e:
        print "Caught the correct type of exception:", e
        g = CameraIsOffExImpl(exception=e)
        g.Print()
    except Exception, e:
        print "Caught the wrong type of exception:", e

    try:
        raise SchedulerAlreadyRunningExImpl
    except SYSTEMErr.SchedulerAlreadyRunningEx, e:
        print "Caught the correct type of exception:", e
        g = SchedulerAlreadyRunningExImpl(exception=e)
        g.Print()
    except Exception, e:
        print "Caught the wrong type of exception:", e

    try:
        raise SchedulerAlreadyStoppedExImpl
    except SYSTEMErr.SchedulerAlreadyStoppedEx, e:
        print "Caught the correct type of exception:", e
        g = SchedulerAlreadyStoppedExImpl(exception=e)
        g.Print()
    except Exception, e:
        print "Caught the wrong type of exception:", e

    try:
        raise NoProposalExecutingExImpl
    except SYSTEMErr.NoProposalExecutingEx, e:
        print "Caught the correct type of exception:", e
        g = NoProposalExecutingExImpl(exception=e)
        g.Print()
    except Exception, e:
        print "Caught the wrong type of exception:", e

    try:
        raise SystemInAutoModeExImpl
    except SYSTEMErr.SystemInAutoModeEx, e:
        print "Caught the correct type of exception:", e
        g = SystemInAutoModeExImpl(exception=e)
        g.Print()
    except Exception, e:
        print "Caught the wrong type of exception:", e

    try:
        raise CannotOpenDeviceExImpl
    except SYSTEMErr.CannotOpenDeviceEx, e:
        print "Caught the correct type of exception:", e
        g = CannotOpenDeviceExImpl(exception=e)
        g.Print()
    except Exception, e:
        print "Caught the wrong type of exception:", e


    joe = AlreadyInAutomaticCompletionImpl()
    joe.Print()


    joe = PositionOutOfLimitsCompletionImpl()
    joe.Print()


    joe = ProposalNotYetReadyCompletionImpl()
    joe.Print()


    joe = InvalidProposalStatusTransitionCompletionImpl()
    joe.Print()


    joe = ImageAlreadyStoredCompletionImpl()
    joe.Print()


    joe = CameraIsOffCompletionImpl()
    joe.Print()


    joe = SchedulerAlreadyRunningCompletionImpl()
    joe.Print()


    joe = SchedulerAlreadyStoppedCompletionImpl()
    joe.Print()


    joe = NoProposalExecutingCompletionImpl()
    joe.Print()


    joe = SystemInAutoModeCompletionImpl()
    joe.Print()


    joe = CannotOpenDeviceCompletionImpl()
    joe.Print()


    print
