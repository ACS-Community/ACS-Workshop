#include "SchedulerImpl.h"

///Include the acs thread header 
#include <acsThread.h>

Scheduler::Scheduler (
	 const ACE_CString& name,
	 maci::ContainerServices *& containerServices)
: ACSComponentImpl(name, containerServices),
  m_schedulerLoop_p(NULL)
{
}

Scheduler::~Scheduler ()
{
}

void Scheduler::execute()
{
  ACS_SHORT_LOG((LM_INFO,"Scheduler::execute"));

  instrument_p = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT");
  telescope_p  = getContainerServices()->getComponent<TELESCOPE_MODULE::Telescope>("TELESCOPE");
  database_p = getContainerServices()->getComponent<DATABASE_MODULE::DataBase>("DATABASE");

  if (instrument_p == INSTRUMENT_MODULE::Instrument::_nil() )
    ACS_SHORT_LOG((LM_ERROR,"Scheduler::execute: cant retrieve component Instrument"));

  if (telescope_p == TELESCOPE_MODULE::Telescope::_nil() )
    ACS_SHORT_LOG((LM_ERROR,"Scheduler::execute: cant retrieve component Telescope"));

  if (database_p == DATABASE_MODULE::DataBase::_nil() )
    ACS_SHORT_LOG((LM_ERROR,"Scheduler::execute: cant retrieve component DataBase"));

  Scheduler * selfPtr = this;
  m_schedulerLoop_p = getContainerServices()->getThreadManager()->
                      create<SchedulerThread, Scheduler*>("schedulerLoop", selfPtr);
}


void Scheduler::cleanUp()
{
    ACS_TRACE("Scheduler::cleanUp");

    if (instrument_p != INSTRUMENT_MODULE::Instrument::_nil() ) {
      getContainerServices()->releaseComponent("INSTRUMENT");
      instrument_p = INSTRUMENT_MODULE::Instrument::_nil();
    }
    if (telescope_p != TELESCOPE_MODULE::Telescope::_nil() ) {
      getContainerServices()->releaseComponent("TELESCOPE");
      telescope_p = TELESCOPE_MODULE::Telescope::_nil();
    }
    if (database_p != DATABASE_MODULE::DataBase::_nil() ) {
      getContainerServices()->releaseComponent("DATABASE");
      database_p = DATABASE_MODULE::DataBase::_nil();
    }
    // Here we have to stop all threads
    getContainerServices()->getThreadManager()->stopAll();
    if (m_schedulerLoop_p != NULL) {
      m_schedulerLoop_p->please_stop_early = true; // Flag for the work thread to stop observations early
      getContainerServices()->getThreadManager()->destroy(m_schedulerLoop_p);
      m_schedulerLoop_p = NULL;
    }
}

void Scheduler::start (void)
{
  ACS_SHORT_LOG((LM_INFO,"Scheduler::start"));
  if (m_schedulerLoop_p != NULL) {
    // Restart observations by resuming the work thread
    // If it is already running, give an error (it means we were called twice).
    if (m_schedulerLoop_p->isSuspended()) {
      ACS_SHORT_LOG((LM_INFO,"Scheduler::start: Resume work thread."));
      m_schedulerLoop_p->please_stop_early = false;  // Initialize stop-early flag to false
      m_schedulerLoop_p->resume();
    } else
      ACS_SHORT_LOG((LM_ERROR,"Scheduler::start: work thread is already running"));
  }  else
    ACS_SHORT_LOG((LM_ERROR,"Scheduler::start: work thread not started"));
}

void Scheduler::stop (void)
{
  ACS_SHORT_LOG((LM_INFO,"Scheduler::stop"));
  if (m_schedulerLoop_p != NULL) {
    if (!m_schedulerLoop_p->isSuspended()) {
      // Pause observations by suspending the work thread
      // If it is already suspended, give an error (it means we were called twice).
      ACS_SHORT_LOG((LM_INFO,"Scheduler::stop: Suspend work thread."));
      m_schedulerLoop_p->please_stop_early = true; // Flag for the work thread to stop observations early
      m_schedulerLoop_p->suspend();
    } else
      ACS_SHORT_LOG((LM_ERROR,"Scheduler::start: work thread is already running"));
  } else
    ACS_SHORT_LOG((LM_ERROR,"Scheduler::stop: work thread not started"));
}

::CORBA::Long Scheduler::proposalUnderExecution (void)
{
  return 0;
}

void SchedulerThread::onStart()
{
  ACS_SHORT_LOG((LM_INFO,"SchedulerThread: started"));
}

void SchedulerThread::onStop()
{
  ACS_SHORT_LOG((LM_INFO,"SchedulerThread: stopped"));
}

void SchedulerThread::runLoop()
{
  if (loopCounter_m < 10)
    ACS_SHORT_LOG((LM_INFO,"SchedulerThread::runLoop (start) %d", loopCounter_m));
  for (int target = 0; target < 5; target++) {
    if (please_stop_early) {
      ACS_SHORT_LOG((LM_INFO,"SchedulerThread::runLoop stop early"));
      break;
    }
    ACS_SHORT_LOG((LM_INFO,"SchedulerThread::runLoop observing target number %d", target));
    ACE_OS::sleep(1);
  }
  if (loopCounter_m < 10)
    ACS_SHORT_LOG((LM_INFO,"SchedulerThread::runLoop (stop) %d", loopCounter_m));
  loopCounter_m++;
}

#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(Scheduler)
