#include <telescope.h>
#include <iostream>
using namespace std;


TelescopeImpl::TelescopeImpl(const ACE_CString& name,
                                            maci::ContainerServices* containerServices){
	        //component_name = name.c_str();
	        //component_running = true;

}

TelescopeImpl::~TelescopeImpl(){
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
