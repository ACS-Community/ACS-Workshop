#include <Telescope2.h>
#include <cmath>
#include <stdlib.h>     //for using the function sleep
#include <string>


//Alarm System
#include <ACSAlarmSystemInterfaceFactory.h>
#include <faultStateConstants.h>

using namespace acsalarm;

TelescopeImpl::TelescopeImpl(const ACE_CString& name,
                                            maci::ContainerServices* containerServices):
	ACSComponentImpl(name, containerServices)
{
}

TelescopeImpl::~TelescopeImpl()
{
}

void TelescopeImpl::initialize(void){
        mount = TELESCOPE_MODULE::TelescopeControl::_nil();
        mount =  getContainerServices()->getDefaultComponent<TELESCOPE_MODULE::TelescopeControl>("IDL:acsws/TELESCOPE_MODULE/TelescopeControl:1.0");


        instrument = INSTRUMENT_MODULE::Instrument::_nil();
        instrument = getContainerServices()->getDefaultComponent<INSTRUMENT_MODULE::Instrument>("IDL:acsws/INSTRUMENT_MODULE/Instrument:1.0");

}

void TelescopeImpl::execute(void){

}

void TelescopeImpl::cleanUp(void){

}

void TelescopeImpl::aboutToAbort(void){
}


TYPES::ImageType* TelescopeImpl::observe(const ::TYPES::Position & coordinates,
      ::CORBA::Long exposureTime)
{

	if(CORBA::is_nil(instrument.in())){
		throw acsErrTypeLifeCycle::LifeCycleExImpl(__FILE__, __LINE__, "::TelescopeImpl::observe");
	}
	//Calibrating Mount	
	mount->calibrateEncoders();	
	//Moving telescope
	moveTo(coordinates);	

	//Calling instrument to take image
	TYPES::ImageType* image; 

	try{
		image = instrument->takeImage(exposureTime);
		return image;

	}catch(SYSTEMErr::CameraIsOffEx &ex){
		std::string family = "Antenna";
		std::string member = "TEST1";

		ACSAlarmSystemInterfaceFactory::createAndSendAlarm(family, member, 1, true);
		//Throw Alarm
		ACS_SHORT_LOG((LM_ERROR, "Camera is off"));

		throw ex;
	}
}



void TelescopeImpl::moveTo( const ::TYPES::Position & coordinates)
{
	
	double max_el = 62;

	double epsilon = 0.5;	
	//Call Mount setTo
	CORBA::Double el = coordinates.el;
	CORBA::Double az = coordinates.az; 


	//Check Position limits
	if(el > max_el){
		SYSTEMErr::PositionOutOfLimitsExImpl ex(__FILE__, __LINE__ , "TelescopeImpl::moveTo");
		ex.log();
		throw ex.getPositionOutOfLimitsEx();
	}


        ACS_SHORT_LOG((LM_INFO,"Moving Mount to el %f az %f", (double) el, (double) az));

        if(CORBA::is_nil(mount.in())){
		throw acsErrTypeLifeCycle::LifeCycleExImpl(__FILE__, __LINE__, "::TelescopeImpl::observe");
	}	
	mount->setTo(el,az);
/*
	//Waiting to get in position
        while(true){
                TYPES::Position actual_p = getCurrentPosition();
                if(std::abs(actual_p.el - el) <= epsilon && std::abs(actual_p.az - az) <= epsilon){
                        break;
                }
		sleep(1);

        }
*/
}

TYPES::Position TelescopeImpl::getCurrentPosition(){

        if(CORBA::is_nil(mount.in())){
                throw acsErrTypeLifeCycle::LifeCycleExImpl(__FILE__, __LINE__, "::TelescopeImpl::observe");
        }
	
	ACS::ROdouble_ptr el = mount->actualAltitude();
	ACS::ROdouble_ptr az = mount->actualAzimuth();

	ACSErr::Completion_var completion;
	
	CORBA::Double el_c = el->get_sync(completion.out());
	CORBA::Double az_c = az->get_sync(completion.out());
		
	TYPES::Position p;
	p.el = el_c;
	p.az = az_c;	
	ACS_SHORT_LOG((LM_DEBUG,"Current position el %f az %f ",(double) el_c, (double) az_c));
	return p;
}

/* --------------- [ MACI DLL support functions ] -----------------*/

#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(TelescopeImpl)
