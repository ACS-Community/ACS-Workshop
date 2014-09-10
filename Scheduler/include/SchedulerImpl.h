#ifndef SchedulerImpl_H
#define SchedulerImpl_H

#include "SchedulerS.h"

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <acscomponentImpl.h>
#include <SchedulerS.h>
#include <InstrumentC.h>
#include <TelescopeC.h>
#include <DataBaseC.h>

class Scheduler: public acscomponent::ACSComponentImpl,
	    public virtual POA_SCHEDULER_MODULE::Scheduler
{
  public:
    /**
     * Constructor
     * 
     * @param name component's name. This is also the name that will be used to find the
     * configuration data for the component in the Configuration Database.
     * @param containerServices The pointer to the ContainerServices
     */
    Scheduler(
	 const ACE_CString& name,
	 maci::ContainerServices *& containerServices);

    /**
     * Destructor
     */
    virtual ~Scheduler();

    virtual void start (void);

    virtual void stop (void);

    virtual ::CORBA::Long proposalUnderExecution (void);

    virtual void execute();

    virtual void cleanUp();

private:
    INSTRUMENT_MODULE::Instrument_var instrument_p;
    TELESCOPE_MODULE::Telescope_var telescope_p;
    DATABASE_MODULE::DataBase_var database_p;
};

#endif /*!SchedulerImpl_H*/
