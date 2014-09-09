#ifndef DataBaseImpl_h
#define DataBaseImpl_h

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <acscomponentImpl.h>
#include <TypesC.h>
#include <SYSTEMErr.h>
#include <DataBaseS.h>

using namespace acscomponent;

class DataBaseImpl:public virtual ACSComponentImpl,
        public virtual POA_DATABASE_MODULE::DataBase{

public:
	DataBaseImpl(const ACE_CString& name, maci::ContainerServices* containerServices);	
	~DataBaseImpl();

	virtual CORBA::Long storeProposal(const TYPES::TargetList& targets);
	virtual CORBA::Long getProposalStatus(const CORBA::Long pid);
	virtual void removeProposal(const CORBA::Long pid);
	virtual TYPES::ImageList *getProposalObservations(const CORBA::Long pid);
	virtual TYPES::ProposalList* getProposals();
	virtual void setProposalStatus(const CORBA::Long pid, const CORBA::Long status);
	virtual void storeImage(CORBA::Long pid, CORBA::Long tid, const TYPES::ImageType& image);
	virtual void clean();
private:
	

};

#endif


