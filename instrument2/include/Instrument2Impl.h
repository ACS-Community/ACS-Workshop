#ifndef _INSTRUMENT2IMPL_H
#define _INSTRUMENT2IMPL_H

#include <maciContainerImpl.h>

//Used to access other components, activate "OffShoot"s, etc.
#include <maciContainerServices.h>

#include <acscomponentImpl.h>

#include <ACSErrTypeCommon.h>

#include "InstrumentS.h"


class Instrument2Impl : public virtual acscomponent::ACSComponentImpl,
		    public POA_INSTRUMENT_MODULE::Instrument
{
  public:

  bool encendido;
  INSTRUMENT_MODULE::Instrument_var Instrument_ref;

  Instrument2Impl(const ACE_CString &name,   maci::ContainerServices * containerServices);
  virtual ~Instrument2Impl();

  virtual void cameraOn ();
  virtual void cameraOff ();
  virtual ::CORBA::Long takeImage();
  virtual void setRGB();
  virtual void setPixelBias();
  virtual void setResetLevel();

};



#endif
