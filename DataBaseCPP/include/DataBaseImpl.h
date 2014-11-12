#ifndef DataBaseImpl_h
#define DataBaseImpl_h

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <acscomponentImpl.h>
#include <TypesC.h>
#include <SYSTEMErr.h>
#include <DataBaseS.h>


struct StructIntImList {
	::CORBA::Long pid,tid;
	TYPES::ImageType_var image;
};

using namespace acscomponent;

class DataBaseImpl:public virtual ACSComponentImpl,
        public virtual POA_DATABASE_MODULE::DataBase{

public:
	DataBaseImpl(const ACE_CString& name, maci::ContainerServices* containerServices);	
	~DataBaseImpl();

	virtual void execute();
	virtual CORBA::Long storeProposal(const TYPES::TargetList& targets);
	virtual CORBA::Long getProposalStatus(const CORBA::Long pid);
	virtual void removeProposal(const CORBA::Long pid);
	virtual TYPES::ImageList *getProposalObservations(const CORBA::Long pid);
	virtual TYPES::ProposalList* getProposals();
	virtual void setProposalStatus(const CORBA::Long pid, const CORBA::Long status);
	virtual void storeImage(CORBA::Long pid, CORBA::Long tid, const TYPES::ImageType& image);
	virtual void clean();
	
private:
	static const long MAXIMUM_IMAGE_BUFFER_SIZE=100;
	::CORBA::Long image_list_ptr;
	::CORBA::ULong currpid;
	TYPES::ProposalList_var pplist;
	struct StructIntImList IntImageList[MAXIMUM_IMAGE_BUFFER_SIZE];	
	Logging::Logger::LoggerSmartPtr firstLoggerSmartPtr;
};




#endif


