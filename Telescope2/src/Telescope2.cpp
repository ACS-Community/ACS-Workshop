#include <Telescope2.h>


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
        mount =  getContainerServices()->getComponent<TELESCOPE_MODULE::TelescopeControl>("MOUNT");


        instrument = INSTRUMENT_MODULE::Instrument::_nil();
        instrument = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT");


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
	
	
	//Moving telescope
	moveTo(coordinates);	

	//Calling instrument to take image
	TYPES::ImageType* image; 
	image = instrument->takeImage(exposureTime);
	
	return image;
}



void TelescopeImpl::moveTo( const ::TYPES::Position & coordinates)
{
	//Check Position limits
	
	//Call Mount setTo
	CORBA::Double el = coordinates.el;
	CORBA::Double az = coordinates.az; 

	mount->setTo(el,az);

	ACS_SHORT_LOG((LM_INFO,"Moving Mount to el %f az %f", (double) el, (double) az));
}

TYPES::Position TelescopeImpl::getCurrentPosition(){
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
