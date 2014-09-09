#ifndef SchedulerImpl_H
#define SchedulerImpl_H

#include "SchedulerS.h"

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <acscomponentImpl.h>
#include <SchedulerS.h>


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
 
};


#endif /*!SchedulerImpl_H*/
