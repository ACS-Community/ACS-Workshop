#include "acscomponentImpl.h"
#include "SchedulerS.h"
#include "DataBaseS.h"
#include "InstrumentS.h"
#include "TelescopeS.h"
#include <SYSTEMErrS.h>

class SchedulerThread;

class SchedulerImpl :
	public acscomponent::ACSComponentImpl,
	public POA_SCHEDULER_MODULE::Scheduler {

public:	

	SchedulerImpl(const ACE_CString &name, maci::ContainerServices *cs);
	~SchedulerImpl();

	// lifecycle methods
	//
	void start() throw (SYSTEMErr::SchedulerAlreadyRunningEx);
	void stop() throw (SYSTEMErr::SchedulerAlreadyStoppedEx);
	CORBA::Long proposalUnderExecution() throw (SYSTEMErr::NoProposalExecutingEx);

	void processProposals();

	bool getProposalToBeObserved();
	void observeProposal();
	//void storeObservations();

private:
	SchedulerThread *m_controlLoop_p;
	DATABASE_MODULE::DataBase_var database_p;
	INSTRUMENT_MODULE::Instrument_var instrument_p;
	TELESCOPE_MODULE::Telescope_var telescope_p;	
	CORBA::Long proposalUnderExecution_m;

	static const int QUEUED;
	static const int RUNNING;
	static const int READY;

	CORBA::Long pid_m;
	TYPES::TargetList targets_m;
	//TYPES::ImageList images_m;
};
