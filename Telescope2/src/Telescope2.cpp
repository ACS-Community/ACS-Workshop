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
	cout << "Observe \n";
	return NULL;
}



void TelescopeImpl::moveTo( const ::TYPES::Position & coordinates)
{
	cout << " Moving \n";
}

TYPES::Position TelescopeImpl::getCurrentPosition(){
	cout << "Position \n";
	return TYPES::Position();
}

/* --------------- [ MACI DLL support functions ] -----------------*/

#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(TelescopeImpl)
