/*******************************************************************************
*    ASTRI ACS Workshop Asiago 2012/07/18
*
*    This library is free software; you can redistribute it and/or
*    modify it under the terms of the GNU Lesser General Public
*    License as published by the Free Software Foundation; either
*    version 2.1 of the License, or (at your option) any later version.
*
*    This library is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General Public
*    License along with this library; if not, write to the Free Software
*    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*
*
* who       when      what
* --------  --------  ----------------------------------------------
* Andre Di Paola
*/
 
#include <InstrumentC.h>
#include <TelescopeC.h>
#include <DataBaseC.h>
#include <TypesC.h>
#include <TypesS.h>
#include <SORTErr.h>
#include <schedulerImpl.h>
#include <ACSErrTypeCommon.h>

//Because of bugs in the ContainerServices...we default to using this
#include <maciContainerImpl.h>

//Used to access other components, activate "OffShoot"s, etc.
#include <maciContainerServices.h>

#include <iostream>
#include <acsThread.h>

using namespace maci;

/* ----------------------------------------------------------------*/
Scheduler::Scheduler( 
		       const ACE_CString &name,
		       maci::ContainerServices * containerServices) :
    ACSComponentImpl(name, containerServices)
{
    // ACS_TRACE is used for debugging purposes
    ACS_SHORT_LOG((LM_TRACE,"::Scheduler::Scheduler"));
    
    m_containerServices = containerServices;
    
    fRun = false; // initialized blocking
    PID = -1;
    
    Scheduler * selfPtr = this;
    m_loop_p = m_containerServices->getThreadManager()->create<SchedulerThread, Scheduler*>("loop", selfPtr);
    m_loop_p->resume();
}
/* ----------------------------------------------------------------*/
Scheduler::~Scheduler()
{
    fRun = false;

    // destroy the thread via ThreadManager
    if(m_loop_p!=0)
	m_containerServices->getThreadManager()->destroy(m_loop_p);
}
/* ----------------------------------------------------------------*/
void
Scheduler::execute()
{
    ACS_SHORT_LOG((LM_INFO,"Building::execute"));
    
    /*
     * Get a reference to the front door
     */    

    // Use container to activate the telescope object
    ACS_SHORT_LOG((LM_INFO, "Getting TELESCOPE component"));
    
    m_telescope_p = TELESCOPE_MODULE::Telescope::_nil();

    m_telescope_p = m_containerServices->getComponent<TELESCOPE_MODULE::Telescope>("TELESCOPE");    

    if (CORBA::is_nil(m_telescope_p.in()))
    {
	throw acsErrTypeLifeCycle::LifeCycleExImpl(__FILE__, __LINE__, "::Building::execute");
    }
    
    // Use container to activate the telescope object
    ACS_SHORT_LOG((LM_INFO, "Getting INSTRUMENT component"));
    
    m_instrument_p = INSTRUMENT_MODULE::Instrument::_nil();

    m_instrument_p = m_containerServices->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT");    

    if (CORBA::is_nil(m_instrument_p.in()))
    {
	throw acsErrTypeLifeCycle::LifeCycleExImpl(__FILE__, __LINE__, "::Building::execute");
    }
    
    // Use container to activate the telescope object
    ACS_SHORT_LOG((LM_INFO, "Getting DATABASE component"));
    
    m_database_p = DATABASE_MODULE::DataBase::_nil();

    m_database_p = m_containerServices->getComponent<DATABASE_MODULE::DataBase>("DATABASE");    

    if (CORBA::is_nil(m_database_p.in()))
    {
	throw acsErrTypeLifeCycle::LifeCycleExImpl(__FILE__, __LINE__, "::Building::execute");
    }
}
/* --------------------- [ CORBA interface ] ----------------------*/
void
Scheduler::cleanUp()
{
    ACS_TRACE("::Building::cleanUp");

    fRun = false;

    // Here we have to stop all threads as soon as the current proposal is finished
    ACS_SHORT_LOG((LM_DEBUG, "Waiting for current proposal to finish"));
    while (PID>=0)
	usleep(100000L);
    ACS_SHORT_LOG((LM_DEBUG, "Waiting for current proposal to finish"));
    m_containerServices->getThreadManager()->stopAll(); 

    if (CORBA::is_nil(m_instrument_p.in()) == false)
    {	    
        ACS_SHORT_LOG((LM_DEBUG,"calling cameraOff on instrument"));
    
        // Execute the method cameraOn of class Instrument
        m_instrument_p->cameraOff();

        ACS_SHORT_LOG((LM_DEBUG,"calling cameraOff on instrument done"));
    } 

    // must release the telescope component
    if(CORBA::is_nil(m_telescope_p.in()) == false)
	{
	ACS_SHORT_LOG((LM_DEBUG, "Releasing TELESCOPE"));

	m_containerServices->releaseComponent("TELESCOPE");

	// be sure to set the reference to nil
	m_telescope_p = TELESCOPE_MODULE::Telescope::_nil();
	}
    
    // must release the telescope component
    if(CORBA::is_nil(m_instrument_p.in()) == false)
	{
	ACS_SHORT_LOG((LM_DEBUG, "Releasing INSTRUMENT"));

	m_containerServices->releaseComponent("INSTRUMENT");

	// be sure to set the reference to nil
	m_instrument_p = INSTRUMENT_MODULE::Instrument::_nil();
	}
    
    // must release the telescope component
    if(CORBA::is_nil(m_database_p.in()) == false)
	{
	ACS_SHORT_LOG((LM_DEBUG, "Releasing DATABASE"));

	m_containerServices->releaseComponent("DATABASE");

	// be sure to set the reference to nil
	m_database_p = DATABASE_MODULE::DataBase::_nil();
	}
    
}
/* --------------------- [ CORBA interface ] ----------------------*/
void
Scheduler::start()
{
    ACS_TRACE("::Scheduler::start");

    if (fRun)    
        throw SORTErr::SchedulerAlreadyRunningExImpl(__FILE__, __LINE__, __PRETTY_FUNCTION__).getSchedulerAlreadyRunningEx();
    else
	fRun = true;

    if (CORBA::is_nil(m_instrument_p.in()) == false)
    {	    
        ACS_SHORT_LOG((LM_DEBUG,"calling cameraOn on instrument"));

        // Execute the method cameraOn of class Instrument
        m_instrument_p->cameraOn();

        ACS_SHORT_LOG((LM_DEBUG,"calling cameraOn on instrument done"));
    } 
}
/* ----------------------------------------------------------------*/
void
Scheduler::stop()
{
    ACS_TRACE("::Scheduler::stop");

    if (!fRun)    
        throw SORTErr::SchedulerAlreadyStoppedExImpl(__FILE__, __LINE__, __PRETTY_FUNCTION__).getSchedulerAlreadyStoppedEx();
    else
	fRun = false;
}
/* ----------------------------------------------------------------*/
CORBA::Long
Scheduler::proposalUnderExecution()
{
    ACS_TRACE("::Scheduler::proposalUnderExecution");
    if (PID<0)
        throw SORTErr::NoProposalExecutingExImpl(__FILE__, __LINE__, __PRETTY_FUNCTION__).getNoProposalExecutingEx();
    else
        return PID;
}

void
Scheduler::SchedulerThread::runLoop()
{
    if (scheduler_p->fRun) {
    	ACS_SHORT_LOG((LM_DEBUG, "Executing thread is active!"));
    	
        // this is just testing code
	try 
	{
   	    TYPES::ProposalList_var tmpprops;
   	    TYPES::ProposalList props;
	    TYPES::ImageType_var img;

  	    ACS_SHORT_LOG((LM_INFO,"getting proposals from database"));
	
		//Always check to ensure the database reference is valid!
	    if (CORBA::is_nil(scheduler_p->m_database_p.in()) == false)
	    {	    
		ACS_SHORT_LOG((LM_DEBUG,"calling getProposals on database"));
	    
    		// Execute the method getProposals() of class DataBase
		tmpprops = scheduler_p->m_database_p->getProposals();
		props = tmpprops;
		if (props.length() == 0) {
		    ACS_SHORT_LOG((LM_WARNING,"empty proposals list"));
                    scheduler_p->PID = -1;
		    usleep(2000000L);
		    return;
		}
		// select execution proposal
		scheduler_p->PID = props[0].pid;
		scheduler_p->m_database_p->setProposalStatus(scheduler_p->PID,1L);

		ACS_SHORT_LOG((LM_DEBUG,"calling getProposal on database done"));
	    } 
	    
	    // execute the first proposal
	    for (CORBA::ULong i=0; i<props[0].targets.length(); i++)
	    {
   	        ACS_SHORT_LOG((LM_INFO,"executing target %u coordinates (%.3f,%.3f) exp %u",i,props[0].targets[i].coordinates.az,props[0].targets[i].coordinates.el,props[0].targets[i].expTime));

    		    //Always check to ensure the telescope reference is valid!
		if (CORBA::is_nil(scheduler_p->m_telescope_p.in()) == false)
		{	    
		    ACS_SHORT_LOG((LM_DEBUG,"calling exposure on telescope"));
	    
    		    // Execute the method observe() of class Telescope
		    img = scheduler_p->m_telescope_p->observe(props[0].targets[i].coordinates,props[0].targets[i].expTime);

		    ACS_SHORT_LOG((LM_DEBUG,"calling exposure on telescope done"));
		} 

    		    //Always check to ensure the database reference is valid!
		if (CORBA::is_nil(scheduler_p->m_database_p.in()) == false)
		{	    
		    ACS_SHORT_LOG((LM_DEBUG,"calling storeImage on database"));
	    
    		    // Execute the method observe() of class Telescope
		    scheduler_p->m_database_p->storeImage(props[0].pid,props[0].targets[i].tid,img.in());

		    ACS_SHORT_LOG((LM_DEBUG,"calling storeImage on database done"));
		} 

	    }
	    
   	    scheduler_p->m_database_p->setProposalStatus(scheduler_p->PID,2L);
	}
	catch(...)
	{
	    // TBD: Exception handling 
	    ACS_SHORT_LOG((LM_ERROR, "Execution FAILED going to retry"));
	}

    } else {
        ACS_SHORT_LOG((LM_DEBUG, "Executing thread is waiting to be activated"));
	usleep(2000000L);
    }
    // clears the running PID (it also signals for scheduler idle state)
    scheduler_p->PID = -1;
}

/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(Scheduler)
/* ----------------------------------------------------------------*/


/*___oOo___*/



