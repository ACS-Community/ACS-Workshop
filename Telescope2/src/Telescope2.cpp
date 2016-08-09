#include <Telescope2.h>
#include <iostream>
using namespace std;
TelescopeImpl::TelescopeImpl(const ACE_CString& name,
                                            maci::ContainerServices* containerServices):
	ACSComponentImpl(name, containerServices)
{
}

TelescopeImpl::~TelescopeImpl()
{
}

void TelescopeImpl::initialize(void){
}

void TelescopeImpl::execute(void){
}

void TelescopeImpl::cleanUp(void){

}

void TelescopeImpl::aboutToAbort(void){
}

TYPES::ImageType* TelescopeImpl::observe(const ::TYPES::Position & coordinates,
      ::CORBA::Long exposureTime)
{
	ACS_SHORT_LOG((LM_WARNING, "Not Implemented Yet"));
	return NULL;
}



void TelescopeImpl::moveTo( const ::TYPES::Position & coordinates)
{
	ACS_SHORT_LOG((LM_WARNING, "Not Implemented Yet"));
}

TYPES::Position TelescopeImpl::getCurrentPosition(){
	ACS_SHORT_LOG((LM_WARNING, "Not Implemented Yet"));
	return TYPES::Position();
}

/* --------------- [ MACI DLL support functions ] -----------------*/

#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(TelescopeImpl)
