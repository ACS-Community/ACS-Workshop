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
