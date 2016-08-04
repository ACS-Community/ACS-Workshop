#ifndef _SCHEDULER_IMPL_H_
#define _SCHEDULER_IMPL_H_


#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <acscomponentImpl.h>
#include <SchedulerS.h>
#include <InstrumentC.h>
#include <TelescopeC.h>
#include <DataBaseC.h>

class SchedulerThread;

class SchedulerImpl: public acscomponent::ACSComponentImpl, public virtual POA_SCHEDULER_MODULE::Scheduler
{
  public:
	SchedulerImpl(const ACE_CString& name, maci::ContainerServices*& containerServices);
	virtual void schedule();
	virtual ~SchedulerImpl();
	//ACSComponentImpl interface
	virtual void execute();
	virtual void cleanUp();
	//Scheduler interface
	virtual void start (void);
	virtual void stop (void);
	virtual ::CORBA::Long proposalUnderExecution (void);
  protected:
	virtual void initializeScheduler();
	virtual void destroyScheduler();
	virtual bool isSchedulerReady();
	INSTRUMENT_MODULE::Instrument_var m_instrument_p;
	TELESCOPE_MODULE::Telescope_var m_telescope_p;
	DATABASE_MODULE::DataBase_var m_database_p;
	SchedulerThread* m_schedulerLoop_p;
	CORBA::Long m_executingProposalId; 
	bool m_running;
};

/**
 * The class SchedulerThread is a thread used by the Scheduler class
 * to schedule and run the proposals.
 */
class SchedulerThread : public ACS::Thread
{
  public:
	SchedulerThread(const ACE_CString& name, SchedulerImpl* scheduler, const ACS::TimeInterval& responseTime=ThreadBase::defaultResponseTime, const ACS::TimeInterval& sleepTime=ThreadBase::defaultSleepTime);
	~SchedulerThread();
  protected:
	virtual void runLoop();
	int loopCounter_m;
	SchedulerImpl* m_scheduler_p;
};

#endif //_SCHEDULER_IMPL_H_
