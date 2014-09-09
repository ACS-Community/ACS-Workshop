

// JA! y ahora??? //


#include "TelescopeImpl.h"


TelescopeImpl::TelescopeImpl(const ACE_CString& name, maci::ContainerServices *& containerServices):
  POA_TELESCOPE_MODULE::Telescope(),
  acscomponent::ACSComponentImpl(name, containerServices)
 {
   
   Pos.el = 90.0;
   Pos.az = 0.0;
}


TelescopeImpl::~TelescopeImpl() {

}

TYPES::ImageType * 
TelescopeImpl::observe (
			const ::TYPES::Position & coordinates,
			::CORBA::Long exposureTime) {

  return NULL;

}



void
TelescopeImpl::moveTo (
		       const ::TYPES::Position & coordinates){

  this->Pos = coordinates;
  return;
}


TYPES::Position
TelescopeImpl::getCurrentPosition (void) {
 
  return this->Pos;
}

MACI_DLL_SUPPORT_FUNCTIONS (TelescopeImpl);
