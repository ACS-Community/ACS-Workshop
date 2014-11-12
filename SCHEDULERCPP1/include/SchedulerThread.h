#include <acsThread.h>

class SchedulerImpl;

using ACS::ThreadBase;

class SchedulerThread : public ACS::Thread
{
public:
	SchedulerThread(const ACE_CString& name, 
		SchedulerImpl* schedulerImpl, 
		const ACS::TimeInterval& responseTime=ThreadBase::defaultResponseTime, 
		const ACS::TimeInterval& sleepTime=ThreadBase::defaultSleepTime) :
		ACS::Thread(name) , iShouldTerminate(false)
		{
			ACS_TRACE("SchedulerThread::SchedulerThread");
			loopCounter_m = 0;
			schedulerImpl_p = schedulerImpl;
		}

	~SchedulerThread() { ACS_TRACE("SchedulerThread::~SchedulerThread"); }

	virtual void runLoop();

	bool iShouldTerminate;

private:
	int loopCounter_m;
	SchedulerImpl * schedulerImpl_p;	
};
