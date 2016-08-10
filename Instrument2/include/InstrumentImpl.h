#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <acscomponentImpl.h>

///CORBA generated servant stub
#include <InstrumentS.h>
#include <CameraS.h>

#include <baciSmartPropertyPointer.h>
#include <baciRWstring.h>

class Instrument : public acscomponent::ACSComponentImpl,    //Standard component superclass
		     public virtual POA_INSTRUMENT_MODULE::Instrument    //CORBA servant stub
{
private:
	bool on;
	CAMERA_MODULE::Camera_var camera;

public:
	Instrument(const ACE_CString &name, maci::ContainerServices * containerServices);

	virtual ~Instrument() {};

	virtual void cameraOn();

	virtual void cameraOff();

	virtual ::TYPES::ImageType * takeImage (::CORBA::Long exposureTime);

	virtual void setRGB (const ::TYPES::RGB & rgbConfig);
	
	virtual void setPixelBias(::CORBA::Long bias);

	virtual void setResetLevel(::CORBA::Long resetLevel);

	virtual void initialize() throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl);

	virtual void execute() throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl);

	virtual void cleanUp();

	virtual void aboutToAbort();

	void operator=(const Instrument& other)
	{
		on = other.on;	
	}
};
