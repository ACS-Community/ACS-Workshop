#include <DataBaseImpl.h>

DataBaseImpl::DataBaseImpl(const ACE_CString &name,
			   maci::ContainerServices * containerServices) : acscomponent::ACSComponentImpl(name, containerServices), pplist(new TYPES::ProposalList)
{
	currpid = 0;
	pplist->length(0);
	image_list_ptr=0;
	firstLoggerSmartPtr = getLogger();
	firstLoggerSmartPtr->setName("FirstLogger");
	ACS_LOG(LM_FULL_INFO,__PRETTY_FUNCTION__,(LM_NOTICE,"Test of LM_NOTICE log"));
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
	ACS_LOG(LM_FULL_INFO,__PRETTY_FUNCTION__,(LM_NOTICE,"DataBaseImpl::storeProposal started"));
	TYPES::Proposal newprop;
	newprop.targets = targets;
	newprop.pid = currpid;
	currpid ++;
	newprop.status = 0;
	pplist->length(pplist->length()+1);
	pplist[pplist->length()-1] =newprop;
	ACS_SHORT_LOG((LM_INFO,"storeProposal succeeded to proposal id: %d",newprop.pid));
        return 0L;
}

CORBA::Long DataBaseImpl::getProposalStatus(const CORBA::Long pid)
{
	ACS_TRACE("DataBaseImpl::getProposalStatus");
	ACS_LOG(LM_FULL_INFO,__PRETTY_FUNCTION__,(LM_NOTICE,"DataBaseImpl::getProposalStatus started"));
	CORBA::Long status=0;
	for(unsigned a=0;a<pplist->length();a++){
		if(pplist[a].pid == pid) {
			status = pplist[a].status;
			ACS_SHORT_LOG((LM_INFO,"getProposalStatus succeeded. (proposal): %d",pid));
			break;
		}
	}
        return status;
}

void DataBaseImpl::removeProposal(const CORBA::Long pid)
{
	ACS_TRACE("DataBaseImpl::removeProposal");
	ACS_LOG(LM_FULL_INFO,__PRETTY_FUNCTION__,(LM_NOTICE,"DataBaseImpl::removeStatus started"));
	for(unsigned a=0;a<pplist->length();a++){
		if(pplist[a].pid == pid) {
			pplist[a].pid = 0;
			pplist[a].status = 0;
			pplist[a].targets = 0;
			ACS_SHORT_LOG((LM_INFO,"removeProposal succeeded. (proposal): %d",pid));
			break;
		}
	}
	return;
}

TYPES::ImageList *DataBaseImpl::getProposalObservations(const CORBA::Long pid)
{
	ACS_LOG(LM_FULL_INFO,__PRETTY_FUNCTION__,(LM_NOTICE,"DataBaseImpl::getProposalObservations started"));
	ACS_TRACE("DataBaseImpl::getProposalObservations");
	TYPES::ImageList_var imglist = new TYPES::ImageList;
	int nbimg=0;
	for(int a=0;a<MAXIMUM_IMAGE_BUFFER_SIZE;a++){
		if(IntImageList[a].pid==pid){
		  if(pplist[a].status != 2){
		    ACS_SHORT_LOG((LM_INFO,"getProposalObservations failed. (proposal): %d",pid));

		    SYSTEMErr::ProposalNotYetReadyEx ex;
		    SYSTEMErr::ProposalNotYetReadyExImpl excep(ex);
		    throw excep.getProposalNotYetReadyEx();
		  }else{
		    imglist[nbimg] = IntImageList[a].pid;//image.ptr();
			nbimg++;
			ACS_SHORT_LOG((LM_INFO,"getProposalObservations succeeded. (proposal): %d",pid));
		  }
		}
	}
	return imglist._retn();
}

TYPES::ProposalList* DataBaseImpl::getProposals()
{
	ACS_TRACE("DataBaseImpl::getProposals");
	ACS_LOG(LM_FULL_INFO,__PRETTY_FUNCTION__,(LM_NOTICE,"DataBaseImpl::getProposals started"));
	if(pplist->length()==0){
	  ACS_SHORT_LOG((LM_INFO,"getProposals failed. Proposal list is empty."));
	  return new TYPES::ProposalList;
	}
	return pplist._retn();

}

void DataBaseImpl::setProposalStatus(const CORBA::Long pid, const CORBA::Long status)
{
	ACS_TRACE("DataBaseImpl::setProposalStatus");
	ACS_LOG(LM_FULL_INFO,__PRETTY_FUNCTION__,(LM_NOTICE,"DataBaseImpl::setProposalStatus started"));

	for(unsigned a=0;a<pplist->length();a++){
	  if(pplist[a].pid == pid) {
	    if(status - pplist[a].status != 1){
	      ACS_LOG(LM_FULL_INFO,__PRETTY_FUNCTION__,(LM_NOTICE,"setProposalStatus not succeeded."));

	      SYSTEMErr::InvalidProposalStatusTransitionEx ex;
	      SYSTEMErr::InvalidProposalStatusTransitionExImpl excep(ex);
	      throw excep.getInvalidProposalStatusTransitionEx();

	      return;
	    }else{
	      pplist[a].status = status;
	      ACS_LOG(LM_FULL_INFO,__PRETTY_FUNCTION__,(LM_NOTICE,"setProposalStatus succeeded."));
	    }
	    break;
	  }
	}
	ACS_SHORT_LOG((LM_INFO,"setProposalStatus[%d]: %d",pid,status));
	return;
}

void DataBaseImpl::storeImage(CORBA::Long pid, CORBA::Long tid, const TYPES::ImageType& image)
{
	ACS_TRACE("DataBaseImpl::storeImage");
	ACS_LOG(LM_FULL_INFO,__PRETTY_FUNCTION__,(LM_NOTICE,"DataBaseImpl::storeImage started"));
	for(::CORBA::Long a=0;a<image_list_ptr;a++){
	  if(IntImageList[a].pid==pid&&IntImageList[a].tid==tid){
	    ACS_SHORT_LOG((LM_INFO,"storeImage failed. Image already stored. (proposal,target): %d, %d",pid,tid));

	    SYSTEMErr::ImageAlreadyStoredEx ex;
	    SYSTEMErr::ImageAlreadyStoredExImpl excep(ex);
	    throw excep.getImageAlreadyStoredEx();

	    return;
	  }
	}

	IntImageList[image_list_ptr].image = image;
	IntImageList[image_list_ptr].pid = pid;
	IntImageList[image_list_ptr].tid = tid;
	image_list_ptr ++;

	ACS_SHORT_LOG((LM_INFO,"storeImage succeeded. (proposal,target): %d, %d",pid,tid));
	return;
}

void DataBaseImpl::clean()
{
	ACS_TRACE("DataBaseImpl::clean");
	return;
}


#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS (DataBaseImpl)
