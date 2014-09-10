#include "SchedulerImpl.h"

Scheduler::Scheduler (
	 const ACE_CString& name,
	 maci::ContainerServices *& containerServices)
: ACSComponentImpl(name, containerServices)
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

void Scheduler::start (void)
{
}

void Scheduler::stop (void)
{
}

::CORBA::Long Scheduler::proposalUnderExecution (void)
{
  return 0;
}

#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(Scheduler)
