#include <telescopeImpl.h>
#include <ACSErrTypeCommon.h>
#include <assert.h>
#include <maciContainerServices.h>
#include <maciContainerImpl.h>
#include <math.h>
#include <unistd.h>

ACE_RCSID(Nexstar, telescopeImpl, "$Id: telescopeImpl.cpp,v 1.8 2010-11-19 19:14:52 nexstar Exp $");

Telescope::Telescope( 
	     const ACE_CString& name,
	     maci::ContainerServices * containerServices):
	     ACSComponentImpl(name, containerServices)
{

}

Telescope::~Telescope()
{
}

void Telescope::cleanUp()
{
    AUTO_TRACE(__PRETTY_FUNCTION__);

    if(CORBA::is_nil(telescopeControl) == false)
	{
	ACS_LOG(LM_RUNTIME_CONTEXT, "::TELESCOPE::cleanUp",
		(LM_DEBUG, "Releasing /TELESCOPE"));


	getContainerServices()->releaseComponent("H3E");

	telescopeControl = H3E::TelescopeControl::_nil();
	}
   if(CORBA::is_nil(instrumentControl) == false)
   	{
   	ACS_LOG(LM_RUNTIME_CONTEXT, "::TELESCOPE::cleanUp",
   		(LM_DEBUG, "Releasing /INSTRUMENT"));


   	getContainerServices()->releaseComponent("INSTRUMENT");

   	instrumentControl = INSTRUMENT_MODULE::Instrument::_nil();
   	}


}

void Telescope::execute()
{
	 getTelescopeControl();
	 getInstrumentControl();
}

void Telescope::getTelescopeControl(){
	AUTO_TRACE(__PRETTY_FUNCTION__);
	ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_TRACE,"Calling TelescopeControl Component"));
	try{
	telescopeControl = getContainerServices()->getComponent<H3E::TelescopeControl>("H3E");
	}catch(maciErrType::CannotGetComponentExImpl &_ex){
		telescopeControlState = DOWN;
		ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_TRACE,"Telescope Component Down"));
		return;
	}
	telescopeControlState = ON;
	ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_TRACE,"Telescope Component Up"));
}

void Telescope::getInstrumentControl(){
	AUTO_TRACE(__PRETTY_FUNCTION__);
    ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_TRACE,"Calling Instrument Component"));
    try{
  	instrumentControl = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT");
    }catch(maciErrType::CannotGetComponentExImpl &_ex){
    	instrumentControlState = DOWN;
    	ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_TRACE,"Instrument Component Down"));
    	return;
    }
    instrumentControlState = ON;
    ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_TRACE,"Component Up"));
}


/* --------------------- [ CORBA interface ] ----------------------*/

void Telescope::moveTo (const ::TYPES::Position & coordinates)
{
	AUTO_TRACE(__PRETTY_FUNCTION__);
	if (telescopeControlState == ON){
		if (coordinates.el < 0 && coordinates.az < 0)
			throw new SORTErr::PositionOutOfLimitsEx();

		ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_TRACE,"Start Telescope movement"));
		telescopeControl->setTo(coordinates.el , coordinates.az);
		ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_TRACE,"Telescope Moving"));
		ACSErr::Completion_var completion;
		ACS::ROdouble_var actualAltitude;
		ACS::ROdouble_var actualAzimuth;
		CORBA::Double alt;
		CORBA::Double az;
		actualAltitude = telescopeControl->actualAltitude();
		actualAzimuth = telescopeControl->actualAzimuth();


		if (actualAltitude.ptr() != ACS::ROdouble::_nil() && actualAzimuth.ptr() != ACS::ROdouble::_nil())
		{
			do{
				alt = actualAltitude->get_sync(completion.out());
				az = actualAzimuth->get_sync(completion.out());
				ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_INFO,"Telescope is moving"));
				usleep(500);
			}while(fabsl(alt-coordinates.el)>1 || fabsl(az-coordinates.az)>1);
			ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_INFO,"Telescope in position"));
		}
		else{
			ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_TRACE,"Telescope not moved, memory allocation error"));
		}
	}
	else{
		ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_INFO,"Component Down, move is not possible"));
	}

}



::TYPES::Position
Telescope::getCurrentPosition()
{
	AUTO_TRACE(__PRETTY_FUNCTION__);
	TYPES::Position pos;
	if (telescopeControlState == ON){
		ACSErr::Completion_var completion;

		ACS::ROdouble_var actualAltitude = telescopeControl->actualAltitude();
		ACS::ROdouble_var actualAzimuth = telescopeControl->actualAzimuth();
		if (actualAltitude.ptr() != ACS::ROdouble::_nil() && actualAzimuth.ptr() != ACS::ROdouble::_nil())
	    {
			pos.el = actualAltitude->get_sync(completion.out());
			pos.az = actualAzimuth->get_sync(completion.out());
	    }
	}
	else{
		ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_INFO,"Component Down, move is not possible"));
	}
	return pos;
}

::TYPES::ImageType *
Telescope::observe (
		const ::TYPES::Position & coordinates,
        ::CORBA::Long exposureTime)
{
	AUTO_TRACE(__PRETTY_FUNCTION__);
	TYPES::ImageType *image = new TYPES::ImageType();
	if (instrumentControlState == ON){
		try{
			moveTo(coordinates);
		}
		catch(SORTErr::PositionOutOfLimitsEx &ex){
			throw ex;
		}
		ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_TRACE,"Getting image"));
		try{
			image = instrumentControl->takeImage(exposureTime);
		}
		catch(SORTErr::CameraIsOffEx &err){
			ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_TRACE,"Error the camera is not done"));

		}
		ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_TRACE,"Image taken"));
	}
	else{
		ACS_LOG( LM_FULL_INFO, __PRETTY_FUNCTION__ ,(LM_TRACE,"Cannot Observe, camera component down"));
	}
		return image;
}


/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(Telescope)
/* ----------------------------------------------------------------*/





