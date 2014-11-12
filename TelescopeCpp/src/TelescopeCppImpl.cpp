#include "TelescopeCppImpl.h"

#include <TypesC.h>

#include <acscomponentImpl.h>
#include <unistd.h>
#include <SYSTEMErrS.h>
#include <SYSTEMErr.h>

TelescopeCppImpl::~TelescopeCppImpl()
{}

TelescopeCppImpl::TelescopeCppImpl(const ACE_CString &name, maci::ContainerServices *cs)
: ACSComponentImpl(name, cs)
{
//this->instrument_p = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT");
this->pos = TYPES::Position();
this->pos.az = -99.0;
this->pos.el = -99.0;
}

TYPES::ImageType* TelescopeCppImpl::observe(const ::TYPES::Position & coordinates,
	      ::CORBA::Long exposureTime) throw(SYSTEMErr::PositionOutOfLimitsEx)
{
	ACS_SHORT_LOG((LM_INFO, "Observe Start"));
	this->instrument_p = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT");

	this->moveTo(coordinates);
	/*const ::CORBA::Long bufferlength = 20;
	::CORBA::ULong max = 1000;
	::CORBA::ULong length = bufferlength;
	::CORBA::Long* buffer = new ::CORBA::Long[bufferlength];
	::CORBA::Boolean release = false;
	for(int i=0;i<bufferlength;i++)
		buffer[i] = (::CORBA::Long)i;
	TYPES::ImageType* img = new TYPES::ImageType(max, length,buffer,release);
	*/
	//TYPES::ImageType* img = this->instrument_p->takeImage(exposureTime);

	//return img;
	ACS_SHORT_LOG((LM_DEBUG, "Taking image at instrument"));
	TYPES::ImageType* img;
	try{
		img = this->instrument_p->takeImage(exposureTime);
	}catch (SYSTEMErr::CameraIsOffEx &e){
		ACS_SHORT_LOG((LM_ERROR, "Camera is off"));
		SYSTEMErr::PositionOutOfLimitsExImpl ex2(e, __FILE__, __LINE__, "observe");
		throw ex2.getPositionOutOfLimitsEx();
	} catch(...) {
		SYSTEMErr::PositionOutOfLimitsExImpl ex2(__FILE__, __LINE__, "observe all");
		throw ex2.getPositionOutOfLimitsEx();
	}
	return img;
}

void TelescopeCppImpl::moveTo (const ::TYPES::Position & coordinates) throw(SYSTEMErr::PositionOutOfLimitsEx)
{
	ACS_SHORT_LOG((LM_INFO, "Moving"));
	if((coordinates.az > 360) || (coordinates.az < 0) ||(coordinates.el > 90) || (coordinates.el < 0))
		{
		ACS_SHORT_LOG((LM_ERROR, "TelescopeCppImpl::observe out of limits"));
		SYSTEMErr::PositionOutOfLimitsExImpl e(__FILE__, __LINE__, "observe");
		throw e.getPositionOutOfLimitsEx();
		}
	unsigned int usdelay = 1000000;
	/*while(this->pos.el != coordinates.el && this->pos.az != coordinates.az){
		if (this->pos.el != coordinates.el)
	}*/
	usleep(usdelay);
	this->pos.el = coordinates.el;
	this->pos.az = coordinates.az;
	ACS_SHORT_LOG((LM_INFO, "Done moving"));
}

TYPES::Position TelescopeCppImpl::getCurrentPosition (void)
{
	ACS_SHORT_LOG((LM_INFO, "TelescopeCppImpl::getCurrentPosition"));
	return this->pos;
}


#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(TelescopeCppImpl)
