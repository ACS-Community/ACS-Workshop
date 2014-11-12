#include "SchedulerImpl.h"
#include "SchedulerThread.h"
#include <TypesS.h>
#include <SYSTEMErr.h>

const int SchedulerImpl::QUEUED = 0;
const int SchedulerImpl::RUNNING = 1;
const int SchedulerImpl::READY = 2;

SchedulerImpl::SchedulerImpl(const ACE_CString &name, maci::ContainerServices *cs)
: ACSComponentImpl(name, cs),
m_controlLoop_p(0),
pid_m(-1)
{
	ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::SchedulerImpl"));
}

SchedulerImpl::~SchedulerImpl()
{
	ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::~SchedulerImpl"));
	// destroy the thread via ThreadManager
	if(m_controlLoop_p!=0)
		getContainerServices()->getThreadManager()->destroy(m_controlLoop_p);
}

void SchedulerImpl::start() throw (SYSTEMErr::SchedulerAlreadyRunningEx)
{
	ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::start"));

	// starts the thread
	// if the scheduler has never been started before, we start the thread.  otherwise the 
 	// scheduler raise an exception.
 	if(m_controlLoop_p == 0)
	{
		ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::start creating a thread"));
		SchedulerImpl* selfPtr = this;
		m_controlLoop_p = getContainerServices()->getThreadManager()->
			create<SchedulerThread, SchedulerImpl*>("schedulerControlLoop", selfPtr);
		m_controlLoop_p->setSleepTime(10000000);
                m_controlLoop_p->resume();
	}
	else if(m_controlLoop_p->iShouldTerminate)
	{
		ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::start resuming the thread"));
		m_controlLoop_p->iShouldTerminate = false;
	}
	else
	{
		ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::start was already called"));
                SYSTEMErr::SchedulerAlreadyRunningExImpl ex(__FILE__, __LINE__, "called twice");
                throw ex.getSchedulerAlreadyRunningEx();
	}
}

void SchedulerImpl::stop() throw (SYSTEMErr::SchedulerAlreadyStoppedEx)
{
	ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::stop"));

	// stop the thread
	if(m_controlLoop_p == 0)
	{
		ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::stop thread not created"));
		// TODO: raise an exception
	}
	else if(m_controlLoop_p->iShouldTerminate)
	{
		ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::stop already asked to stop"));
		SYSTEMErr::SchedulerAlreadyStoppedExImpl ex(__FILE__, __LINE__, "called twice");
		throw ex.getSchedulerAlreadyStoppedEx();
	}
	else
	{
		//m_controlLoop_p->suspend();
		m_controlLoop_p->iShouldTerminate = true;
	}
}

CORBA::Long SchedulerImpl::proposalUnderExecution() throw (SYSTEMErr::NoProposalExecutingEx)
{
	ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::proposalUnderExecution"));
	if(pid_m == -1)
	{
		ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::proposalUnderExecution no proposal under execution"));
		SYSTEMErr::NoProposalExecutingExImpl ex(__FILE__, __LINE__, "no proposal is executing");
		throw ex.getNoProposalExecutingEx();
	}
	else
	{
		return pid_m;
	}
}

void SchedulerImpl::processProposals()
{
	ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::processProposals"));

	if(getProposalToBeObserved())
	{
		observeProposal();
		pid_m = -1; // proposal has been observed
	}
}

bool SchedulerImpl::getProposalToBeObserved()
{
	ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::getProposalToBeObserved"));

        database_p = getContainerServices()->getComponent<DATABASE_MODULE::DataBase>("DATABASE");
        ACS_SHORT_LOG((LM_INFO, "SchedulerImpl::getProposalToBeObserved get list of proposals"));
        TYPES::ProposalList_var proposals = database_p->getProposals();
        unsigned int i = 0;
        bool found = false;
        while(i < proposals->length() && !found) {
                ACS_SHORT_LOG((LM_INFO, "SchedulerImpl::processProposals proposal %d", i));
                if(proposals[i].status == QUEUED) {
                        found = true;
                        pid_m = proposals[i].pid;
                        targets_m = proposals[i].targets;
                }
                ++i;
        }
        if(!found)
        {
                ACS_SHORT_LOG((LM_INFO, "SchedulerImpl::getProposalToBeObserved no proposal available to observe"));
                return false;
        }
        ACS_SHORT_LOG((LM_INFO, "SchedulerImpl:getProposalToBeObserved proposal %d available to observe", pid_m));
        try {
                database_p->setProposalStatus(pid_m, RUNNING);
        }
        catch (SYSTEMErr::InvalidProposalStatusTransitionEx &e) {
                ACS_SHORT_LOG((LM_ERROR, "proposal status transition error"));
		pid_m = -1;
                return false;
        }
	return true;
}

void SchedulerImpl::observeProposal()
{
	ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::observeProposal"));

	instrument_p = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT");
	instrument_p->cameraOn();

	telescope_p = getContainerServices()->getComponent<TELESCOPE_MODULE::Telescope>("TELESCOPE");
	ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::observeProposal proposal %d: %d targets found", pid_m, targets_m.length()));
	for(unsigned int i = 0; i < targets_m.length(); ++i) {
		int tid = targets_m[i].tid;
		TYPES::ImageType_var image;
		try {
			image = telescope_p->observe(targets_m[i].coordinates, targets_m[i].expTime);
		}
		catch(SYSTEMErr::PositionOutOfLimitsEx &e) {
			ACS_SHORT_LOG((LM_ERROR, "do not get the image for proposal %d target %d", pid_m, tid));
		}
		database_p->storeImage(pid_m, tid, image);
	}

        instrument_p->cameraOff();
        try {
                database_p->setProposalStatus(pid_m, READY);
        }
        catch (SYSTEMErr::InvalidProposalStatusTransitionEx &e) {
                ACS_SHORT_LOG((LM_ERROR, "proposal status transition error"));
                return;
        }
	ACS_SHORT_LOG((LM_INFO,"SchedulerImpl::observeProposal proposal observed"));
}

#include "maciACSComponentDefines.h"
MACI_DLL_SUPPORT_FUNCTIONS(SchedulerImpl)
