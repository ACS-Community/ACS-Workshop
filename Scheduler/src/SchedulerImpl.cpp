#include "SchedulerImpl.h"

///Include the acs thread header 
#include <acsThread.h>
#include <SYSTEMErr.h>

SchedulerImpl::SchedulerImpl (
	 const ACE_CString& name,
	 maci::ContainerServices *& containerServices):
	ACSComponentImpl(name, containerServices),
	m_schedulerLoop_p(NULL)
{
}

SchedulerImpl::~SchedulerImpl ()
{
}

void SchedulerImpl::execute()
{
	ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::execute"));
	initializeScheduler();
}


void SchedulerImpl::cleanUp()
{
	ACS_TRACE("SchedulerImpl::cleanUp");
	destroyScheduler();
}

void SchedulerImpl::initializeScheduler() {
	ACS_TRACE("SchedulerImpl::initializeScheduler");
	try {
		if (m_instrument_p == INSTRUMENT_MODULE::Instrument::_nil()) {
			m_instrument_p = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT");
		}
	} catch (maciErrType::CannotGetComponentExImpl& ex) {
		ex.log();
		ACS_SHORT_LOG((LM_ERROR,"SchedulerImpl::initializeScheduler: Couldn't retrieve component Instrument."));
	}
	try {
		if (m_telescope_p == TELESCOPE_MODULE::Telescope::_nil()) {
			m_telescope_p = getContainerServices()->getComponent<TELESCOPE_MODULE::Telescope>("TELESCOPE");
		}
	} catch (maciErrType::CannotGetComponentExImpl& ex) {
		ex.log();
		ACS_SHORT_LOG((LM_ERROR,"SchedulerImpl::initializeScheduler: Couldn't retrieve component Telescope."));
	}
	try {
		if (m_database_p == DATABASE_MODULE::DataBase::_nil()) {
			m_database_p = getContainerServices()->getComponent<DATABASE_MODULE::DataBase>("DATABASE");
		}
	} catch (maciErrType::CannotGetComponentExImpl& ex) {
		ex.log();
		ACS_SHORT_LOG((LM_ERROR,"SchedulerImpl::initializeScheduler: Couldn't retrieve component DataBase."));
	}

	if (getContainerServices()->getThreadManager()->getThreadByName("schedulerLoop") == NULL) {
		SchedulerImpl* ptr = this;
		long responseTime=200000000; //20 seconds to wait when destroying the thread.
		long sleepTime=50000000; //5 seconds between scheduling loop.
		m_schedulerLoop_p = getContainerServices()->getThreadManager()->create<SchedulerThread, SchedulerImpl*>("schedulerLoop", ptr, responseTime, sleepTime);
		if (m_schedulerLoop_p == NULL) {
			ACS_SHORT_LOG((LM_ERROR,"SchedulerImpl::initializeScheduler: Couldn't create SchedulerThread."));
		}
	}
}

void SchedulerImpl::destroyScheduler() {
	ACS_TRACE("SchedulerImpl::destroyScheduler");
	if (m_instrument_p != INSTRUMENT_MODULE::Instrument::_nil()) {
		getContainerServices()->releaseComponent("INSTRUMENT");
		m_instrument_p = INSTRUMENT_MODULE::Instrument::_nil();
	}
	if (m_telescope_p != TELESCOPE_MODULE::Telescope::_nil()) {
		getContainerServices()->releaseComponent("TELESCOPE");
		m_telescope_p = TELESCOPE_MODULE::Telescope::_nil();
	}
	if (m_database_p != DATABASE_MODULE::DataBase::_nil()) {
		getContainerServices()->releaseComponent("DATABASE");
		m_database_p = DATABASE_MODULE::DataBase::_nil();
	}
	// Here we have to stop the thread
	if (m_schedulerLoop_p != NULL) {
		m_schedulerLoop_p->stop();
		m_running = false;
		getContainerServices()->getThreadManager()->destroy(m_schedulerLoop_p);
		m_schedulerLoop_p = NULL;
	}
}

bool SchedulerImpl::isSchedulerReady() {
	if (m_instrument_p == INSTRUMENT_MODULE::Instrument::_nil()) {
		return false;
	}
	if (m_telescope_p != TELESCOPE_MODULE::Telescope::_nil()) {
		return false;
	}
	if (m_database_p != DATABASE_MODULE::DataBase::_nil()) {
		return false;
	}
	if (m_schedulerLoop_p == NULL) {
		return false;
	}
	return true;
}

void SchedulerImpl::schedule() {
	// Get proposals from DataBase
	TYPES::ProposalList_var proposals = m_database_p->getProposals();

	int p_len = proposals->length(); // number of proposals
	int p; // proposal counter
	if (p_len > 0)
		ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::schedule: Get proposals (%d)", p_len));

	// Loop over the proposals
	int nProps = 0;
	for (p = 0; p < p_len; p++) {
		if (m_running) {
			ACS_SHORT_LOG((LM_INFO,"SchedulerThread::schedule: Stopped early"));
			break;
		}
		TYPES::Proposal proposal = proposals[p];
		m_executingProposalId = proposal.pid;
		ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::schedule: Observing proposal number %d", proposal.pid));
		try {
			m_database_p->setProposalStatus(proposal.pid, 2); // running
		} catch (SYSTEMErr::InvalidProposalStatusTransitionEx ex) {
			SYSTEMErr::InvalidProposalStatusTransitionExImpl e(ex);
			e.log();
			ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::schedule: Proposal id %d is in incorrect state. Skipping proposal.", proposal.pid));
			continue;
		}
		int t_len = proposal.targets.length(); // number of targets in this proposal
		int t; // target counter
		// Loop over targets
		for (t = 0; t < t_len; t++) {
			TYPES::Target target = proposal.targets[t];
			ACS_SHORT_LOG((LM_INFO,"SchedulerThread::schedule observing target number %d", target.tid));
			m_instrument_p->cameraOn();
			TYPES::ImageType_var image;
			try {
				image = m_telescope_p->observe(target.coordinates, target.expTime);
			} catch (SYSTEMErr::PositionOutOfLimitsEx ex) {
				SYSTEMErr::PositionOutOfLimitsExImpl e(ex);
				e.log();
				ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::schedule: Proposal id %d with target id %d has positions out of limits. Skipping target.", proposal.pid, target.tid));
				continue;
			}
			ACE_OS::sleep(1);
			m_instrument_p->cameraOff();
			try {
				m_database_p->storeImage(proposal.pid, target.tid, image.in());
			} catch (SYSTEMErr::ImageAlreadyStoredEx ex) {
				SYSTEMErr::ImageAlreadyStoredExImpl e(ex);
				e.log();
				ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::schedule: Proposal id %d with target id %d already has an image stored. Skipping target.", proposal.pid, target.tid));
				continue;
			}
		}
		try {
			m_database_p->setProposalStatus(proposal.pid, 3); // done
		} catch (SYSTEMErr::InvalidProposalStatusTransitionEx ex) {
			SYSTEMErr::InvalidProposalStatusTransitionExImpl e(ex);
			e.log();
			ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::schedule: Proposal id %d is in incorrect state. Skipping proposal.", proposal.pid));
			continue;
		}
		nProps++;
	}
	m_executingProposalId = -1;
	if (p_len > 0)
		ACS_SHORT_LOG((LM_INFO,"SchedulerThread::schedule: finished processing proposals (%d)", nProps));
}

void SchedulerImpl::start (void)
{
	ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::start"));
	if (!isSchedulerReady()) {
		ACS_SHORT_LOG((LM_WARNING,"SchedulerImpl::start: Trying to start work thread without proper references to components. Reinitializing scheduler."));
		initializeScheduler();
		if (!isSchedulerReady()) {
			ACS_SHORT_LOG((LM_ERROR,"SchedulerImpl::start: Scheduler references failed to be obtained. Aborting start of the work thread"));
			return;
		} else {
			ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::start: Scheduler references were correctly obtained. Continuing with the start of the work thread"));
		}
	}
	if (m_schedulerLoop_p != NULL) {
		// Restart observations by resuming the work thread
		// If it is already running, give an error (it means we were called twice).
		if (m_schedulerLoop_p->isSuspended()) {
			ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::start: Resume work thread."));
			m_running = true;
			m_schedulerLoop_p->resume();
		} else {
			ACS_SHORT_LOG((LM_WARNING,"SchedulerImpl::start: work thread is already running"));
			SYSTEMErr::SchedulerAlreadyRunningExImpl ex(__FILE__,__LINE__,"SchedulerImpl::start");
			throw ex.getSchedulerAlreadyRunningEx();
		}
	} else {
		ACS_SHORT_LOG((LM_ERROR,"SchedulerImpl::start: work thread not started"));
	}
}

void SchedulerImpl::stop (void)
{
	ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::stop"));
	if (m_schedulerLoop_p != NULL) {
		if (!m_schedulerLoop_p->isSuspended()) {
			// Pause observations by suspending the work thread
			// If it is already suspended, give an error (it means we were called twice).
			ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::stop: Suspend work thread."));
			m_running = false;
			m_schedulerLoop_p->suspend();
		} else {
			ACS_SHORT_LOG((LM_WARNING,"SchedulerImpl::start: work thread is already stopped"));
			SYSTEMErr::SchedulerAlreadyStoppedExImpl ex(__FILE__,__LINE__,"SchedulerImpl::stop");
			throw ex.getSchedulerAlreadyStoppedEx();
		}
	} else {
		ACS_SHORT_LOG((LM_ERROR,"SchedulerImpl::stop: work thread not started"));
	}
}

::CORBA::Long SchedulerImpl::proposalUnderExecution (void)
{
	if (m_executingProposalId == -1) {
		SYSTEMErr::NoProposalExecutingExImpl ex(__FILE__,__LINE__,"SchedulerImpl::proposalUnderExecution");
		throw ex.getNoProposalExecutingEx();
	}
	return m_executingProposalId;
}

//SchedulerThread Implementation
SchedulerThread::SchedulerThread(const ACE_CString& name, SchedulerImpl* scheduler, const ACS::TimeInterval& responseTime, const ACS::TimeInterval& sleepTime):
	ACS::Thread(name, responseTime, sleepTime)
{
	ACS_TRACE("SchedulerThread::SchedulerThread");
	loopCounter_m = 0;
	m_scheduler_p = scheduler;
}

SchedulerThread::~SchedulerThread()
{
	ACS_TRACE("SchedulerThread::~SchedulerThread");
}

void SchedulerThread::runLoop()
{
	loopCounter_m++;
	m_scheduler_p->schedule();
	ACS_SHORT_LOG((LM_INFO,"SchedulerThread::runLoop: Finished scheduling loop (%d).", loopCounter_m));
	ACE_OS::sleep(5);
}

#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(SchedulerImpl)
