

// JA! y ahora??? //


#include "TelescopeImpl.h"


TelescopeImpl::TelescopeImpl(const ACE_CString& name, maci::ContainerServices *& containerServices):
  POA_TELESCOPE_MODULE::Telescope(),
  acscomponent::ACSComponentImpl(name, containerServices)
 {

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

  return;
}


TYPES::Position
TelescopeImpl::getCurrentPosition (void) {
  TYPES::Position Pos;


  return Pos;
}

