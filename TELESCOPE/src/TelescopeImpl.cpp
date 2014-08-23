/*******************************************************************************
*    ASTRI Astrofisica a Specchi con Tecnologia Replicante Italiana
*
*    Authors: Trifoglio - Conforti
*    Date: 18 July 2012

*/
 
#include <TelescopeImpl.h>
#include <TelescopeS.h>
#include <ACSErrTypeCommon.h>
#include <iostream>
#include <TypesC.h>
#include <logging.h>
#include <SORTErr.h>
#include <InstrumentC.h>
#include <maciContainerServices.h>

/* ----------------------------------------------------------------*/
TelescopeImpl::TelescopeImpl(
		       const ACE_CString &name,
		       maci::ContainerServices * containerServices) :
    ACSComponentImpl(name, containerServices),
    cs(containerServices)
{

	ACS_SHORT_LOG ((LM_INFO, "TELESCOPE: Initialization ... started."));


	// define the limit of elevation and azimuth
	minAz = 0.0;
	maxAz = 359.9;
	minEl = 45.0;
	maxEl = 89.0;


	// initialize the el and az
	privateEl = minEl;
	privateAz = minAz;

	ACS_SHORT_LOG ((LM_INFO, "TELESCOPE: Initialization ... completed."));

}
/* ----------------------------------------------------------------*/
TelescopeImpl::~TelescopeImpl()
{
}
/* --------------------- [ CORBA interface ] ----------------------*/
TYPES::Position TelescopeImpl::getCurrentPosition()
{

	TYPES::Position_var pos = new TYPES::Position;
	pos->el = privateEl;
	pos->az = privateAz;

	ACS_SHORT_LOG ((LM_INFO, "TELESCOPE: getCurrentPosition ... started."));

	ACS_SHORT_LOG ((LM_INFO, "Positioned at coordinates: %f %f", privateEl , privateAz));

	ACS_SHORT_LOG ((LM_INFO, "TELESCOPE: getCurrentPosition ... stopped."));

    return pos.out();
//    		CORBA::String_var thomas = CORBA::string_dup("thomas was here");
//    int willy = 5;

}








TYPES::ImageType*
TelescopeImpl::observe(const TYPES::Position& coordinates, CORBA::Long exposureTime){

	ACS_SHORT_LOG ((LM_INFO, "TELESCOPE: observe ... started."));

	TYPES::ImageType_var image = new TYPES::ImageType;

	INSTRUMENT_MODULE::Instrument_var instrument;
	try
	{
		instrument = cs->getComponent< INSTRUMENT_MODULE::Instrument >("INSTRUMENT");
		if(CORBA::is_nil(instrument) == true)
		{
			return image._retn();
		}
	}
	catch(const ACSErrTypeCORBA::NarrowFailedExImpl& ex)
	{
		return image._retn();
	}

	moveTo(coordinates);



	try
	{
		image = instrument->takeImage(exposureTime);
	}
	catch(const SORTErr::CameraIsOffEx& ex)
	{
		SORTErr::CameraIsOffExImpl(ex).log();
	}

	ACS_SHORT_LOG ((LM_INFO, "TELESCOPE: observe ... completed."));

	return image._retn();
}



void TelescopeImpl::moveTo(const TYPES::Position& coordinates){

	int error = 0;

	ACS_SHORT_LOG ((LM_INFO, "TELESCOPE: moveTo ... started."));

	// return exception if el is out of limit
	if ( (coordinates.el < minEl) || (coordinates.el > maxEl) ){
		// throw SORTErr::PositionOutOfLimitsExImpl(__FILE__, __LINE__, __PRETTY_FUNCTION__).getPositionOutOfLimitsEx();
		SORTErr::PositionOutOfLimitsExImpl ex(__FILE__, __LINE__, __PRETTY_FUNCTION__);
		ex.log();
		throw ex.getPositionOutOfLimitsEx();

		error = 1;

	}

	// return exception if el az out of limit
	if ( (coordinates.az < minAz) || (coordinates.az > maxAz) ){
		SORTErr::PositionOutOfLimitsExImpl ex(__FILE__, __LINE__, __PRETTY_FUNCTION__);
		ex.log();
		throw ex.getPositionOutOfLimitsEx();

		error = 1;
	}

	if (error == 0){
		privateEl = coordinates.el;
		privateAz = coordinates.az;
		ACS_SHORT_LOG((LM_INFO, "moved to the required position"));
	}else{
		ACS_SHORT_LOG((LM_INFO, "I cannot move the telescope because out of limits"));
	}

	ACS_SHORT_LOG ((LM_INFO, "TELESCOPE: moveTo ... completed."));

	//@TODO

	// set the private variabile of position with the passed coordinates

//	/raises(SORTErr::PositionOutOfLimitsEx);


}




/* ----------------------------------------------------------------*/


/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(TelescopeImpl)
/* ----------------------------------------------------------------*/


/*___oOo___*/



