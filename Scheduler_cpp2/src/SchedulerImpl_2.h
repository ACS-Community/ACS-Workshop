#include "SchedulerS.h"
#include "TelescopeC.h"
#include "DataBaseC.h"
#include "InstrumentC.h"

#include "acscomponentImpl.h"
#include "SYSTEMErr.h"

class SchedulerImpl_2: public acscomponent::ACSComponentImpl,
		public POA_SCHEDULER_MODULE::Scheduler {
public:
	class LoopThroughProposalsThread;
	SchedulerImpl_2(const ACE_CString &name, maci::ContainerServices *cs);
	~SchedulerImpl_2();
	void start(void) throw(SYSTEMErr::SchedulerAlreadyRunningEx);
	void stop(void) throw(SYSTEMErr::SchedulerAlreadyStoppedEx);
	::CORBA::Long proposalUnderExecution(void) throw (SYSTEMErr::NoProposalExecutingEx);
	
	void execute(); //getting references
	void cleanUp(); //deleting references


protected:
	::CORBA::Long propId_;
	bool isStarted_;

	DATABASE_MODULE::DataBase_var dataBaseComponent_;
	INSTRUMENT_MODULE::Instrument_var instrumentComponent_;
	TELESCOPE_MODULE::Telescope_var telescopeComponent_;
public:
	LoopThroughProposalsThread* controlLoop_;
	::TYPES::ProposalList_var propList_;
	bool askedForStop_;
};

class SchedulerImpl_2::LoopThroughProposalsThread : public ACS::Thread
{
public:
	LoopThroughProposalsThread(const ACE_CString& name, 
		SchedulerImpl_2* scheduler, 
		const ACS::TimeInterval& responseTime=ThreadBase::defaultResponseTime, 
		const ACS::TimeInterval& sleepTime=ThreadBase::defaultSleepTime) :
		ACS::Thread(name)
	{
		ACS_TRACE("LoopThroughProposalsThread::LoopThroughProposalsThread");
		scheduler_ = scheduler;
	}

	~LoopThroughProposalsThread() { ACS_TRACE("LoopThroughProposalsThread::~LoopThroughProposalsThread"); }

	virtual void runLoop();
protected:
	SchedulerImpl_2* scheduler_;
};

