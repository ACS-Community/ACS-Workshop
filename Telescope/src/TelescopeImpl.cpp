



#include "TelescopeImpl.h"
#include <ACSErrTypeCORBA.h>
#include <ErrTelescope.h>


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
  TYPES::ImageType * image = NULL;
  moveTo(coordinates);
  
  if (inst_p == INSTRUMENT_MODULE::Instrument::_nil() ) {    
    /*  ACS_SHORT_LOG((LM_ERROR,
		   "TelescopeImpl::observe: cant retrive component Instrument"));
    */

    ACSErrTypeCORBA::CORBAReferenceNilExImpl ex(__FILE__, __LINE__,"TelescopeImpl::observe");
    ex.setVariable("Instrument");
    ex.log();
    throw ex.getCORBAReferenceNilEx();
  } else {
    ACS_SHORT_LOG((LM_INFO,"TelescopeImpl::observe"));
    image = inst_p->takeImage(exposureTime);
  }
  
  return image;
}

// Manejo de errores si somos usuarios de observe():
/*
try {
observe()
  } catch(CORBAReferenceNilEx ex) {
  ex.log()
 }
  } catch (...) {
 }
*/
void TelescopeImpl::moveTo (const ::TYPES::Position & coordinates)
{
  ErrTelescope::ErrTelescopeCOOROORExImpl ex(__FILE__, __LINE__,"TelescopeImpl::moveTo");

  if ( coordinates.el < 0 || coordinates.el > 90 ) {
    ex.setElevation(coordinates.el);
    ex.log();
    throw ex.getErrTelescopeCOOROOREx();
  }
  if ( coordinates.az < 0 || coordinates.az > 360 ) {
    ex.setAzimut(coordinates.az);
    ex.log();
    throw ex.getErrTelescopeCOOROOREx();
  }

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


#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS (TelescopeImpl);
