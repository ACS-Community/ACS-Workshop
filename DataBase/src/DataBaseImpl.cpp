#include <DataBaseImpl.h>

DataBaseImpl::DataBaseImpl(const ACE_CString &name,
        maci::ContainerServices * containerServices) : acscomponent::ACSComponentImpl(name, containerServices)
{

}

DataBaseImpl::~DataBaseImpl()
{
}

CORBA::Long DataBaseImpl::storeProposal(const TYPES::TargetList& targets)
{
	return 0;
}

CORBA::Long DataBaseImpl::getProposalStatus(const CORBA::Long pid)
{
	return 0;
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
	TYPES::ProposalList* proposalList = NULL;
	return proposalList;
}

void DataBaseImpl::setProposalStatus(const CORBA::Long pid, const CORBA::Long status)
{
}

void DataBaseImpl::storeImage(CORBA::Long pid, CORBA::Long tid, const TYPES::ImageType& image)
{
}

void DataBaseImpl::clean()
{
}


#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS (DataBaseImpl)
