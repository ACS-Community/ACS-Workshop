#include <DataBaseImpl.h>

DataBaseImpl::DataBaseImpl(const ACE_CString &name,
			   maci::ContainerServices * containerServices) : acscomponent::ACSComponentImpl(name, containerServices), pplist(new TYPES::ProposalList)
{
	currpid = 0;
	pplist->length(0);
	image_list_ptr=0;

}

DataBaseImpl::~DataBaseImpl()
{
}

void DataBaseImpl::execute()
{
	ACS_TRACE("DataBaseImpl::execute");
}

CORBA::Long DataBaseImpl::storeProposal(const TYPES::TargetList &targets)
{
	ACS_TRACE("DataBaseImpl::storeProposal");
	TYPES::Proposal currentproposal;
	::CORBA::ULong curlen;
	//Validate targets!!!

	//Increment current proposal ID
	currentproposal.pid=currpid++;
	currentproposal.targets = targets ;
	currentproposal.status = TYPES::PROPOSAL_STATUS_QUEUED;

	//// Append the new Proposal to the List
	curlen = pplist->length();
	pplist->length(++curlen);
	pplist[curlen-1] = currentproposal;

	return currentproposal.pid;
}

CORBA::Long DataBaseImpl::getProposalStatus(const CORBA::Long pid)
{
	ACS_TRACE("DataBaseImpl::getProposalStatus");
	//Return the status of the >>pid<< elements in the Proposal List
        TYPES::Proposal p = (*pplist)[pid];
        return p.status;
	//return pplist[pid].status;
}

void DataBaseImpl::removeProposal(const CORBA::Long pid)
{
	ACS_TRACE("DataBaseImpl::removeProposal");
	(*pplist)[pid].status = TYPES::PROPOSAL_STATUS_REMOVED;
	return;
}

TYPES::ImageList *DataBaseImpl::getProposalObservations(const CORBA::Long pid)
{
	ACS_TRACE("DataBaseImpl::getProposalObservations");
        TYPES::ImageList_var imlist(new TYPES::ImageList);
	imlist->length(0);
	::CORBA::ULong curlen;
        for (int ii=0;ii<image_list_ptr;ii++)
		if (IntImageList[ii].pid == pid)
		{
			curlen=imlist->length();
			++curlen;
			imlist->length(curlen);
			imlist[curlen-1]=IntImageList[ii].image;
		}
			
	return imlist._retn();
}

TYPES::ProposalList* DataBaseImpl::getProposals()
{
	ACS_TRACE("DataBaseImpl::getProposals");
	TYPES::ProposalList_var myplist(new TYPES::ProposalList);
	::CORBA::ULong curlen;
	myplist->length(0);
	for (unsigned int ii=0;ii<pplist->length();ii++)
	{
		if ((*pplist)[ii].status == TYPES::PROPOSAL_STATUS_QUEUED)
		{
			curlen = myplist->length();
			myplist->length(++curlen);
			myplist[curlen-1] = pplist[ii];
		}
	}		

	return myplist._retn();

}

void DataBaseImpl::setProposalStatus(const CORBA::Long pid, const CORBA::Long status)
{
	ACS_TRACE("DataBaseImpl::setProposalStatus");
	if (status >= 0 && status < 4) (*pplist)[pid].status = status ;
	return;
}

void DataBaseImpl::storeImage(CORBA::Long pid, CORBA::Long tid, const TYPES::ImageType& image)
{
	ACS_TRACE("DataBaseImpl::storeImage");
	// Append the new Proposal to the List
	IntImageList[image_list_ptr].pid=pid;
	IntImageList[image_list_ptr].tid=tid;
	IntImageList[image_list_ptr].image=image;
	image_list_ptr=++image_list_ptr % MAXIMUM_IMAGE_BUFFER_SIZE;

	return;
}

void DataBaseImpl::clean()
{
	ACS_TRACE("DataBaseImpl::clean");
	for (unsigned int ii=0;ii<pplist->length();ii++)
		(*pplist)[ii].targets.length(0);
	pplist->length(0);
	return;
}


#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS (DataBaseImpl)
