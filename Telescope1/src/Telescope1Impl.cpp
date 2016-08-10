

#include <Telescope1Impl.h>

Telescope1::Telescope1(const ACE_CString& name,
               maci::ContainerServices * containerServices):
               ACSComponentImpl(name, containerServices)
{

   ACS_TRACE("::Telescope1::Telescope1");    
}

Telescope1::~Telescope1(){
}

::TYPES::ImageType * 
Telescope1::observe(const ::TYPES::Position & coordinates,::CORBA::Long exposureTime){
	ACS_SHORT_LOG((LM_INFO,"Telescope1::observe"));
        ::TYPES::Position frompos = getCurrentPosition();
        std::stringstream s;
        s << "Moving telescope from "  << frompos.az << "," << frompos.el << " to "  << coordinates.az << "," << coordinates.el; 
        std::string ss;
        ss = s.str();
        ACS_SHORT_LOG((LM_NOTICE,ss.c_str()))
        s.str("");
        s << "Observing target for "  << exposureTime << " seconds"; 
        ss = s.str();
        ACS_SHORT_LOG((LM_NOTICE,ss.c_str()))
        ::TYPES::ImageType *whatever=new ::TYPES::ImageType();
	return whatever;
}

void 
Telescope1::moveTo(const ::TYPES::Position & coordinates){
	ACS_SHORT_LOG((LM_INFO,"Telescope1::moveTo"));
       // return NULL;
}

::TYPES::Position 
Telescope1::getCurrentPosition(void){
	ACS_SHORT_LOG((LM_INFO,"Telescope1::getCurrentPosition"));
        ::TYPES::Position retval;
        retval.az=90;
        retval.el=15;  
        return retval;
}

/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(Telescope1)
/* ----------------------------------------------------------------*/


