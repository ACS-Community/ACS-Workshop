

#include <Telescope1Impl.h>

Telescope1::Telescope1(const ACE_CString& name,
               maci::ContainerServices * containerServices):
               ACSComponentImpl(name, containerServices)
{
   ACS_TRACE("::Telescope1::Telescope1");
   cmount_p = TELESCOPE_MODULE::TelescopeControl::_nil();
   cmount_p = getContainerServices()->getComponent<TELESCOPE_MODULE::TelescopeControl>("MOUNT");
   
   if (CORBA::is_nil(cmount_p.in())){
        //TODO:: Throw an exception 
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
        

	if(coordinates.el > 75){
		ACS_SHORT_LOG((LM_WARNING,"Not moving to prevent camera damage."));
		SYSTEMErr::PositionOutOfLimitsExImpl error("Telescope1Impl.cpp",34,NULL, DEFAULT_SEVERITY);
		throw error;
	}
	if(coordinates.el > 220 || coordinates.el < -220){
		ACS_SHORT_LOG((LM_WARNING, "Not moving to prevent cable damage."));
		SYSTEMErr::PositionOutOfLimitsExImpl error("Telescope1Impl.cpp",34,NULL, DEFAULT_SEVERITY);
		throw error;	
	}	
	if(coordinates.el < 0){
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
        INSTRUMENT_MODULE::Instrument_var inst_p = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT1");
        s.str("");
        s << "Observing target for "  << exposureTime << " seconds"; 
        ss = s.str();
        ACS_SHORT_LOG((LM_NOTICE,ss.c_str()))
        ::TYPES::ImageType *my_image=inst_p->takeImage(exposureTime);
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
             if(abs(cpos.el - coord.el) < 0.5 && abs(cpos.az - coord.az) < 0.5)
		on_source=true;
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
}

void 
Telescope1::moveTo(const ::TYPES::Position & coordinates) throw (SYSTEMErr::PositionOutOfLimitsEx, CORBA::SystemException){
        ACS_TRACE("::Telescope1::moveTo");
        std::stringstream s;

	if(coordinates.el > 62){
		ACS_SHORT_LOG((LM_WARNING,"Not moving to prevent camera damage."));
		SYSTEMErr::PositionOutOfLimitsExImpl error("Telescope1Impl.cpp",34,NULL, DEFAULT_SEVERITY);
		throw error;
	}
	if(coordinates.el > 220 || coordinates.el < -220){
		ACS_SHORT_LOG((LM_WARNING, "Not moving to prevent cable damage."));
		SYSTEMErr::PositionOutOfLimitsExImpl error("Telescope1Impl.cpp",34,NULL, DEFAULT_SEVERITY);
		throw error;	
	}	
	if(coordinates.el < 0){
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
        ACS_TRACE("::Telescope1::getCurrentPosition");
        ACSErr::Completion *comp = new ACSErr::Completion();
        ::TYPES::Position retval;
        ::ACS::ROdouble_ptr a_alt_p=cmount_p->actualAltitude();
        ::ACS::ROdouble_ptr a_azi_p=cmount_p->actualAzimuth();
        retval.el = a_alt_p->get_sync(comp);
        retval.az = a_azi_p->get_sync(comp);
        return retval;
}

/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(Telescope1)
/* ----------------------------------------------------------------*/


