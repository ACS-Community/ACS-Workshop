
#include <ACSAlarmSystemInterfaceFactory.h>
#include <Telescope1Impl.h>
#include <FaultState.h>
#include <faultStateConstants.h>

using namespace acsalarm;
//::AlarmSystemInterface;

Telescope1::Telescope1(const ACE_CString& name,
               maci::ContainerServices * containerServices):
               ACSComponentImpl(name, containerServices)
{
   ACS_TRACE("::Telescope1::Telescope1");
   cmount_p = TELESCOPE_MODULE::TelescopeControl::_nil();
   //cmount_p = getContainerServices()->getComponent<TELESCOPE_MODULE::TelescopeControl>("MOUNT");
   cmount_p = getContainerServices()->getDefaultComponent<TELESCOPE_MODULE::TelescopeControl>("IDL:acsws/TELESCOPE_MODULE/TelescopeControl:1.0");
   
   if (CORBA::is_nil(cmount_p.in())){
        //TODO:: Throw an exception
	//ALARM
	std::string family = "Telescope1";
	std::string member = "INSTRUMENT";
	int code = 3;

	acsalarm::AlarmSystemInterface* alarmSource = ACSAlarmSystemInterfaceFactory::createSource();
	
		auto_ptr<acsalarm::FaultState> fltstate = ACSAlarmSystemInterfaceFactory::createFaultState(family, member, code);
	
		fltstate -> setDescriptor(faultState::ACTIVE_STRING);
		
		acsalarm::Timestamp * tstampPtr = new acsalarm::Timestamp();
		auto_ptr<acsalarm::Timestamp> tstampAutoPtr(tstampPtr);
		fltstate->setUserTimestamp(tstampAutoPtr);
	
		acsalarm::Properties * propsPtr = new acsalarm::Properties();
		propsPtr->setProperty("STATUS","OFF");
		auto_ptr<acsalarm::Properties> propsAutoPtr(propsPtr);
		fltstate->setUserProperties(propsAutoPtr);

		alarmSource->push(*fltstate);
	//ALARM_END
        ACS_SHORT_LOG((LM_CRITICAL,"MOUNT component cannot be found, please reload the component."));
   }
   cmount_p->calibrateEncoders();
}

Telescope1::~Telescope1(){
   cmount_p->park();
   cmount_p->setUncalibrated();
}

::TYPES::ImageType * 
Telescope1::observe(const ::TYPES::Position & coordinates,::CORBA::Long exposureTime) throw (SYSTEMErr::PositionOutOfLimitsEx, CORBA::SystemException){
	ACS_SHORT_LOG((LM_INFO,"Telescope1::observe"));
        // Get the current position
        ::TYPES::Position frompos = getCurrentPosition();
        std::stringstream s;
        

	if(coordinates.el > 62){
		//ALARM
		std::string family = "Telescope1";
		std::string member = "MOUNT";
		int code = 1;

		acsalarm::AlarmSystemInterface* alarmSource = ACSAlarmSystemInterfaceFactory::createSource();
	
		auto_ptr<acsalarm::FaultState> fltstate = ACSAlarmSystemInterfaceFactory::createFaultState(family, member, code);
	
		fltstate -> setDescriptor(faultState::ACTIVE_STRING);
		
		acsalarm::Timestamp * tstampPtr = new acsalarm::Timestamp();
		auto_ptr<acsalarm::Timestamp> tstampAutoPtr(tstampPtr);
		fltstate->setUserTimestamp(tstampAutoPtr);
	
		acsalarm::Properties * propsPtr = new acsalarm::Properties();
		propsPtr->setProperty("ELEVATION_MAXIMUM","62");
		auto_ptr<acsalarm::Properties> propsAutoPtr(propsPtr);
		fltstate->setUserProperties(propsAutoPtr);

		alarmSource->push(*fltstate);
	
		//ALARM_END

		ACS_SHORT_LOG((LM_WARNING,"Not moving to prevent camera damage."));
		SYSTEMErr::PositionOutOfLimitsExImpl error("Telescope1Impl.cpp",34,NULL, DEFAULT_SEVERITY);
		throw error;
	}
	if(coordinates.el < 0){
		//ALARM
		std::string family = "Telescope1";
		std::string member = "MOUNT";
		int code = 1;

		acsalarm::AlarmSystemInterface* alarmSource = ACSAlarmSystemInterfaceFactory::createSource();
	
		auto_ptr<acsalarm::FaultState> fltstate = ACSAlarmSystemInterfaceFactory::createFaultState(family, member, code);
	
		fltstate -> setDescriptor(faultState::ACTIVE_STRING);
		
		acsalarm::Timestamp * tstampPtr = new acsalarm::Timestamp();
		auto_ptr<acsalarm::Timestamp> tstampAutoPtr(tstampPtr);
		fltstate->setUserTimestamp(tstampAutoPtr);
	
		acsalarm::Properties * propsPtr = new acsalarm::Properties();
		propsPtr->setProperty("ELEVATION_MINIMUM","0");
		auto_ptr<acsalarm::Properties> propsAutoPtr(propsPtr);
		fltstate->setUserProperties(propsAutoPtr);

		alarmSource->push(*fltstate);
		//ALARM_END

		ACS_SHORT_LOG((LM_WARNING, "Not moving to prevent brain damage."));
		SYSTEMErr::PositionOutOfLimitsExImpl error("Telescope1Impl.cpp",34,NULL, DEFAULT_SEVERITY);
		throw error;
	}

        // Move the telescope
        s << "Moving telescope from "  << frompos.az << "," << frompos.el << " to "  << coordinates.az << "," << coordinates.el; 
        std::string ss;
        ss = s.str();
        ACS_SHORT_LOG((LM_NOTICE,ss.c_str()));
        cmount_p->setTo(coordinates.el,coordinates.az);
        
        // Wait the telescope arrive to...
        this->waitOnSource(coordinates);
        // Observe
        //INSTRUMENT_MODULE::Instrument_var inst_p = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT");
        INSTRUMENT_MODULE::Instrument_var inst_p = getContainerServices()->getDefaultComponent<INSTRUMENT_MODULE::Instrument>("IDL:acsws/INSTRUMENT_MODULE/Instrument:1.0");
        s.str("");
        s << "Observing target for "  << exposureTime << " seconds"; 
        ss = s.str();
        ACS_SHORT_LOG((LM_NOTICE,ss.c_str()));
	::TYPES::ImageType *my_image=NULL;
        try{
        	my_image=inst_p->takeImage(exposureTime);
        }
        catch (::SYSTEMErr::CameraIsOffEx ex){
                SYSTEMErr::CameraIsOffExImpl cex(ex);
                cex.log();
                ACS_SHORT_LOG((LM_ERROR,"Aborting... an returning empty image"));
        	my_image=new ::TYPES::ImageType();
	}
	return my_image;
}

void 
Telescope1::waitOnSource(const ::TYPES::Position & coord){
        ACS_TRACE("::Telescope1::waitOnSource");
        ::TYPES::Position cpos; 
        bool on_source=false;
        time_t init=time(NULL);
        while (!on_source){
             cpos=this->getCurrentPosition();
             if(abs(cpos.el - coord.el) < 0.2 && abs(cpos.az - coord.az) < 0.2){
		on_source=true;
	   }
             else{
                usleep(SLEEP_TIME);
                std::stringstream s;
                // Move the telescope
                s << "Telescope at "  << cpos.az << "," << cpos.el << " position, sleeping for " << SLEEP_TIME << " [usec]";
                std::string ss;
                ss = s.str();
                ACS_SHORT_LOG((LM_DEBUG,ss.c_str()))
             }
             if (time(NULL) - init > TIME_OUT){
                ACS_SHORT_LOG((LM_ERROR,"Source not reached due to telescope timeout"));
                //TODO: add the exception           
                break;
	     }
                
	}
	usleep(ON_SOURCE_TIME);
}

void 
Telescope1::moveTo(const ::TYPES::Position & coordinates) throw (SYSTEMErr::PositionOutOfLimitsEx, CORBA::SystemException){


	
        ACS_TRACE("::Telescope1::moveTo");
        std::stringstream s;

	if(coordinates.el > 62){
		//ALARM
		std::string family = "Telescope1";
		std::string member = "MOUNT";
		int code = 1;

		acsalarm::AlarmSystemInterface* alarmSource = ACSAlarmSystemInterfaceFactory::createSource();
	
		auto_ptr<acsalarm::FaultState> fltstate = ACSAlarmSystemInterfaceFactory::createFaultState(family, member, code);
	
		fltstate -> setDescriptor(faultState::ACTIVE_STRING);
		
		acsalarm::Timestamp * tstampPtr = new acsalarm::Timestamp();
		auto_ptr<acsalarm::Timestamp> tstampAutoPtr(tstampPtr);
		fltstate->setUserTimestamp(tstampAutoPtr);
	
		acsalarm::Properties * propsPtr = new acsalarm::Properties();
		propsPtr->setProperty("ELEVATION_MAXIMUM","62");
		auto_ptr<acsalarm::Properties> propsAutoPtr(propsPtr);
		fltstate->setUserProperties(propsAutoPtr);

		alarmSource->push(*fltstate);
		//ALARM_END

		ACS_SHORT_LOG((LM_WARNING,"Not moving to prevent camera damage."));
		SYSTEMErr::PositionOutOfLimitsExImpl error("Telescope1Impl.cpp",34,NULL, DEFAULT_SEVERITY);
		throw error;
	}
	if(coordinates.el < 0){
		//ALARM
		std::string family = "Telescope1";
		std::string member = "MOUNT";
		int code = 1;

		acsalarm::AlarmSystemInterface* alarmSource = ACSAlarmSystemInterfaceFactory::createSource();
	
		auto_ptr<acsalarm::FaultState> fltstate = ACSAlarmSystemInterfaceFactory::createFaultState(family, member, code);
	
		fltstate -> setDescriptor(faultState::ACTIVE_STRING);
		
		acsalarm::Timestamp * tstampPtr = new acsalarm::Timestamp();
		auto_ptr<acsalarm::Timestamp> tstampAutoPtr(tstampPtr);
		fltstate->setUserTimestamp(tstampAutoPtr);
	
		acsalarm::Properties * propsPtr = new acsalarm::Properties();
		propsPtr->setProperty("ELEVATION_MINIMUM","0");
		auto_ptr<acsalarm::Properties> propsAutoPtr(propsPtr);
		fltstate->setUserProperties(propsAutoPtr);

		alarmSource->push(*fltstate);
	
		//ALARM_END
		ACS_SHORT_LOG((LM_WARNING, "Not moving to prevent brain damage."));
		SYSTEMErr::PositionOutOfLimitsExImpl error("Telescope1Impl.cpp",34,NULL, DEFAULT_SEVERITY);
		throw error;
	}	


        // Move the telescope
        s << "Telescope moving to "  << coordinates.az << "," << coordinates.el;
        std::string ss;
        ss = s.str();
	ACS_SHORT_LOG((LM_INFO,ss.c_str()));
        cmount_p->setTo(coordinates.el,coordinates.az);
        this->waitOnSource(coordinates);
}

::TYPES::Position 
Telescope1::getCurrentPosition(void){
	std::stringstream s;

        ACS_TRACE("::Telescope1::getCurrentPosition");
        ACSErr::Completion *comp = new ACSErr::Completion();
        ::TYPES::Position retval;
        ::ACS::ROdouble_ptr a_alt_p=cmount_p->actualAltitude();
        ::ACS::ROdouble_ptr a_azi_p=cmount_p->actualAzimuth();
        retval.el = a_alt_p->get_sync(comp);
        retval.az = a_azi_p->get_sync(comp);

        s << "Telescope position succesfully retrieved: "  << retval.az << "," << retval.el;
        std::string ss;
        ss = s.str();
	ACS_SHORT_LOG((LM_INFO, ss.c_str()));
        return retval;
}

/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(Telescope1)
/* ----------------------------------------------------------------*/


