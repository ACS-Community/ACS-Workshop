#include <DataBaseImpl.h>

DataBaseImpl::DataBaseImpl(const ACE_CString &name,
        maci::ContainerServices * containerServices) : acscomponent::ACSComponentImpl(name, containerServices)
{
	currpid = 0;
	pplist->length(0);
}

DataBaseImpl::~DataBaseImpl()
{
}

CORBA::Long DataBaseImpl::storeProposal(const TYPES::TargetList &targets)
{
	TYPES::Proposal currentproposal;
	::CORBA::ULong curlen;
	//Validate targets!!!

	//Increment current proposal ID
	currentproposal.pid=currpid++;
	currentproposal.targets = targets ;
	currentproposal.status = TYPES::PROPOSAL_STATUS_QUEUED;

	// Append the new Proposal to the List
	curlen = pplist->length(); 
	pplist->length(curlen++);
	pplist[curlen] = currentproposal;

	// Return the Proposal ID
	return currentproposal.pid;
}

CORBA::Long DataBaseImpl::getProposalStatus(const CORBA::Long pid)
{
	//Return the status of the >>pid<< elements in the Proposal List
	return pplist[pid].status;
}

void DataBaseImpl::removeProposal(const CORBA::Long pid)
{
}

TYPES::ImageList *DataBaseImpl::getProposalObservations(const CORBA::Long pid)
{
	TYPES::ImageList *imageList = NULL;
	return imageList;
}

TYPES::ProposalList* DataBaseImpl::getProposals()
{
	return pplist;
}

void DataBaseImpl::setProposalStatus(const CORBA::Long pid, const CORBA::Long status)
{
	if (status >= 0 && status < 3) pplist[pid].status = status ;
	return 0;

}

void DataBaseImpl::storeImage(CORBA::Long pid, CORBA::Long tid, const TYPES::ImageType& image)
{
}

void DataBaseImpl::clean()
{
}


#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS (DataBaseImpl)
