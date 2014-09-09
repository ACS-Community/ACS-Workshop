



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


void TelescopeImpl::execute()
{
  ACS_SHORT_LOG((LM_INFO,"Building::execute"));

  inst_p = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT");

  if (inst_p == INSTRUMENT_MODULE::Instrument::_nil() ) {
    
    ACS_SHORT_LOG((LM_ERROR,"Building::execute: cant retrive component Instrument"));

  }
}


void TelescopeImpl::cleanUp()
{
    ACS_TRACE("TelescopeImpl::cleanUp");


    if (inst_p != INSTRUMENT_MODULE::Instrument::_nil() ) {
      getContainerServices()->releaseComponent("INSTRUMENT");
      
      // be sure to set the reference to nil
      inst_p = INSTRUMENT_MODULE::Instrument::_nil();
    }

}


TYPES::ImageType * 
TelescopeImpl::observe (
			const ::TYPES::Position & coordinates,
			::CORBA::Long exposureTime) 
{
  ACS_SHORT_LOG((LM_INFO,"TelescopeImpl::observe"));
  moveTo(coordinates);
  inst_p->takeImage(exposureTime);

  return NULL;
}


void TelescopeImpl::moveTo (const ::TYPES::Position & coordinates)
{
  ACS_SHORT_LOG((LM_INFO,
		 "TelescopeImpl::moveTo: %.1f, %.1f",
		 coordinates.el, coordinates.az));
  this->Pos = coordinates;
  return;
}


TYPES::Position TelescopeImpl::getCurrentPosition (void) 
{
  return this->Pos;
}


MACI_DLL_SUPPORT_FUNCTIONS (TelescopeImpl);
