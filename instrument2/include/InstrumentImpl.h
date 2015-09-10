#ifndef _INSTRUMENTIMPL_H
#define _INSTRUMENTIMPL_H

#include <acscomponentImpl.h>

#include <ACSErrTypeCommon.h>

#include "InstrumentS.h"


class InstrumentImpl : public virtual acscomponent::ACSComponentImpl,
		    public POA_INSTRUMENT_MODULE::Instrument
{
  public:

  bool encendido;

  InstrumentImpl(const ACE_CString &name,   maci::ContainerServices * containerServices);
  virtual ~InstrumentImpl();

  virtual void cameraOn ();
  virtual void cameraOff ();
  virtual ::CORBA::Long takeImage();
  virtual void setRGB();
  virtual void setPixelBias();
  virtual void setResetLevel();

};



#endif
