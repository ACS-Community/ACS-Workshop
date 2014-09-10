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

class SchedulerThread;

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

    // These are public (in the implementation only) because are needed
    // by the SchedulerThread
    INSTRUMENT_MODULE::Instrument_var instrument_p;
    TELESCOPE_MODULE::Telescope_var telescope_p;
    DATABASE_MODULE::DataBase_var database_p;

private:
    SchedulerThread *m_schedulerLoop_p;
};

/**
 * The class SchedulerThread is a thread used by the Scheduler class
 * to run the proposals.
 */
class SchedulerThread : public ACS::Thread
{
  public:
    SchedulerThread(const ACE_CString& name,
                    Scheduler * scheduler,
                    const ACS::TimeInterval& responseTime=ThreadBase::defaultResponseTime,
                    const ACS::TimeInterval& sleepTime=ThreadBase::defaultSleepTime) :
                    ACS::Thread(name)
    {
      ACS_TRACE("SchedulerThread::SchedulerThread");
      loopCounter_m = 0;
      scheduler_p = scheduler;
    }

    ~SchedulerThread() { ACS_TRACE("SchedulerThread::~SchedulerThread"); }

    virtual void onStart();

    virtual void onStop();

    virtual void runLoop();

  protected:
    int loopCounter_m;
    Scheduler * scheduler_p;
};

#endif /*!SchedulerImpl_H*/
