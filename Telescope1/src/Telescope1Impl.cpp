

#include <Telescope1Impl.h>

Telescope1::Telescope1(const ACE_CString& name,
               maci::ContainerServices * containerServices){

   ACS_TRACE("::Telescope1::Telescope1");    
}

Telescope1::~Telescope1();

::TYPES::ImageType * 
Telescope1::observe(const ::TYPES::Position & coordinates,::CORBA::Long exposureTime){
	ACS_SHORT_LOG((LM_INFO,"Telescope1::observe"));
	return NULL;
}

void 
Telescope1::moveTo(const ::TYPES::Position & coordinates){
	ACS_SHORT_LOG((LM_INFO,"Telescope1::moveTo"));
       // return NULL;
}

::TYPES::Position 
Telescope1::getCurrentPosition(void){
	ACS_SHORT_LOG((LM_INFO,"Telescope1::getCurrentPosition"));
        return NULL;
}


