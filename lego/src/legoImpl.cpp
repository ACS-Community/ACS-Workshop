#include "legoImpl.h"
legoImpl::legoImpl(const ACE_CString& name, maci::ContainerServices * containerServices)
		: ACSComponentImpl(name, containerServices), myCallback("moveto")
{
	legoImpl::camera = INSTRUMENT_MODULE::Instrument::_nil();
	legoImpl::camera = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT");

	legoImpl::antenna = AltAzModule::AltAz::_nil();
	legoImpl::antenna = getContainerServices()->getComponent<AltAzModule::AltAz>("devLego");
	
}
legoImpl::~legoImpl()
{
}

::TYPES::ImageType * legoImpl::observe (const ::TYPES::Position & coordinates,
        	                        ::CORBA::Long exposureTime) 
{
	::TYPES::ImageType * fromCamera;

	ACS_TRACE("legoImpl::moveTo");
	legoImpl::moveTo(coordinates);

	ACS_TRACE("Getting the instrument reference");
		
	ACS_TRACE("Taking the image");
	try { 
		fromCamera = legoImpl::camera->takeImage(exposureTime);
	}
	catch(SORTErr::CameraIsOffExImpl &exx) {
		ACS_ERROR("The camera is off!");
		fromCamera = NULL;
	}
		
	return fromCamera;
}

void legoImpl::moveTo (const ::TYPES::Position & coordinates)
{
	ACS::CBDescIn desc;
        //Activate it as a CORBA object
        legoImpl::cb = myCallback._this();


	ACS_TRACE("legoImpl::moveTo");
	antenna->moveto(cb.in(), desc,coordinates.el,coordinates.az);
}

::TYPES::Position legoImpl::getCurrentPosition (void)
{
	::TYPES::Position currentPosition;
	ACS::ROdouble_var realAz = antenna->realAz();
	ACS::ROdouble_var realAlt = antenna->realAlt();
        ACSErr::Completion_var completion;
        currentPosition.az = realAz->get_sync(completion.out());
	currentPosition.el = realAlt->get_sync(completion.out());
 
	ACS_TRACE("legoImpl::getCurrentPosition");
	
	return currentPosition;
}
