#include "SchedulerThread.h"
#include "SchedulerImpl.h"

int LOCALCOUNT = 0;
ACE_Log_Priority LOCAL_LOGGING_LEVEL = LM_INFO;
const static int MAX_LOGS = 5;

void
SchedulerThread::runLoop()
{
	static bool processing = false;
	//Used to change the logging level to make acsexmpl's modular
	//tests more deterministic.
	if(LOCALCOUNT>MAX_LOGS)
	{
		LOCAL_LOGGING_LEVEL = LM_DEBUG;
	}
	else
	{
		LOCALCOUNT++;
	}
	ACS_SHORT_LOG((LOCAL_LOGGING_LEVEL, "SchedulerThread::runLoop"));

	if(iShouldTerminate)
	{
		//if(LOCALCOUNT < MAX_LOGS)
		if(processing) {
			ACS_SHORT_LOG((LM_INFO, "SchedulerThread::runLoop not processing proposals"));
			processing = false;
		}
		LOCALCOUNT = 0;
		//exit();
	}
	else
	{
		//if(LOCALCOUNT < MAX_LOGS)
		if(!processing) {
			ACS_SHORT_LOG((LM_INFO, "SchedulerThread::runLoop processing proposals"));
			processing = true;
		}
		schedulerImpl_p->processProposals();
	}
}
