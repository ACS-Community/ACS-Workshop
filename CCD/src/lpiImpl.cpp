#include <vltPort.h>
static char *rcsId="@(#) $Id: lpiImpl.cpp,v 1.2 2008/11/25 22:10:26 almamgr Exp $";
static void *use_rcsId = ((void)&use_rcsId,(void *) &rcsId);

#include <stdlib.h>
#include "lpiFrameDevIO.h"
#include "lpiPropsDevIO.h"
#include "lpiImpl.h"
//#include "UOSErr.h"
#include <linux/videodev2.h>


using namespace baci;

/* Constructor */
lpiImpl::lpiImpl(const ACE_CString& name, maci::ContainerServices *containerServices) :
       CharacteristicComponentImpl(name,containerServices)
      ,m_frame_sp(this)
      ,m_device_sp(this)
      ,m_red_sp(this)
      ,m_blue_sp(this)
      ,m_green_sp(this)
      ,m_pixelBias_sp(this)
      ,m_resetLevel_sp(this)
      ,m_exposure_sp(this)
{
	component_name = name.c_str();
	m_device_sp = new RWstring(name+":device", getComponent());
}

/* Destructor */
lpiImpl::~lpiImpl(){
}

/* Component Lifecycle */
void lpiImpl::initialize() throw (acsErrTypeLifeCycle::LifeCycleExImpl)
{

	static char * _METHOD_ = "lpiImpl::initialize";

	ACSErr::Completion_var completion;
	char *deviceName = m_device_sp->get_sync(completion.out());
	int fd;

	ACS_TRACE("lpiImpl::initialize");
	if( getComponent() != 0){

		lpiFrameDevIO *frameDevIO = NULL;
		//try{
			int flag = O_RDWR; //read write flag, is the accesmode you have to put in open
			struct stat st;
			static char * _METHOD_ = "lpiFrameDevIO::lpiFrameDevIO";

			//we see if the deviceName device exist
			//if (-1 == stat (deviceName, &st)) {
			//
			//	char *buffer = new char[strlen("Cannot start device ''") + strlen(deviceName) + 1];
			//	sprintf(buffer,"Cannot start device '%s'",deviceName);
			//
			//	ACS_LOG( LM_ERROR , _METHOD_ , (LM_ERROR, "CannotOpenDeviceEx: %s",buffer) );
			//	UOSErr::CannotOpenDeviceExImpl ex(__FILE__,__LINE__,_METHOD_);
			//	ex.addData("Reason",buffer);
			//
			//	throw ex.getCannotOpenDeviceEx();
			//}


			//if it is a character device
			//if (!S_ISCHR (st.st_mode)) {

			//	char *buffer = new char[strlen("File '' is not a char device") + strlen(deviceName) + 1];
			//	sprintf(buffer,"File '%s' is not a char device",deviceName);

			//	ACS_LOG( LM_ERROR , _METHOD_ , (LM_ERROR, "CannotOpenDeviceEx: %s",buffer) );
			//	UOSErr::CannotOpenDeviceExImpl ex(__FILE__,__LINE__,_METHOD_);
			//	ex.addData("Reason",buffer);

			//	throw ex.getCannotOpenDeviceEx();
			//}

			fd = open(deviceName, flag);

			//if it cannot open
			//if(fd==-1) {

			//	char *buffer = new char[strlen("Cannot open device '' for writing: ") + strlen(deviceName) + strlen(strerror(errno))+ 1];
			//	sprintf(buffer,"Cannot open device '%s' for writing: %s",deviceName,strerror(errno));

			//	ACS_LOG( LM_ERROR , _METHOD_ , (LM_ERROR, "CannotOpenDeviceEx: %s",buffer) );
			//	UOSErr::CannotOpenDeviceExImpl ex(__FILE__,__LINE__,_METHOD_);
			//	ex.addData("Reason",buffer);

			//	throw ex.getCannotOpenDeviceEx();
			//}

			ACS_SHORT_LOG((LM_INFO,"lpiFrameDevIO::lpiFrameDevIO: Video device opened!"));

			frameDevIO = new lpiFrameDevIO(fd, deviceName);
//		} catch (UOSErr::CannotOpenDeviceEx &ex){
//			acsErrTypeLifeCycle::LifeCycleExImpl lifeEx(ex,__FILE__,__LINE__,_METHOD_);
//			lifeEx.addData("Reason","Cannot create DevIOs");
//			throw lifeEx;
//		}

		m_frame_sp = new ROlongSeq( (component_name + std::string(":frame")).c_str(),
				getComponent(), frameDevIO ,true);

		m_red_sp = new RWlong( (component_name + std::string(":red")).c_str(),
				getComponent(), new lpiPropsDevIO(fd,V4L2_CID_BLUE_BALANCE) ,true);
		m_blue_sp = new RWlong( (component_name + std::string(":blue")).c_str(),
				getComponent(), new lpiPropsDevIO(fd,V4L2_CID_RED_BALANCE) ,true);
		m_green_sp = new RWlong( (component_name + std::string(":green")).c_str(),
				getComponent(), new lpiPropsDevIO(fd,SN9C102_V4L2_CID_GREEN_BALANCE) ,true);
		m_pixelBias_sp = new RWlong( (component_name + std::string(":pixelBias")).c_str(),
				getComponent(), new lpiPropsDevIO(fd,SN9C102_V4L2_CID_PIXEL_BIAS_VOLTAGE) ,true);
		m_resetLevel_sp = new RWlong( (component_name + std::string(":resetLevel")).c_str(),
				getComponent(), new lpiPropsDevIO(fd,SN9C102_V4L2_CID_RESET_LEVEL) ,true);
		m_exposure_sp = new RWlong( (component_name + std::string(":exposure")).c_str(),
				getComponent(), new lpiPropsDevIO(fd,V4L2_CID_EXPOSURE) ,true);
	}
}

void lpiImpl::aboutToAbort()
{
	close(fd);
}

/* IDL implementation */

TYPES::ImageType* lpiImpl::image(CORBA::Double exposure) throw (CORBA::SystemException){

	ACSErr::Completion_var comp;
	unsigned int length = 640*480*3;

	ACS::longSeq *frame = m_frame_sp->get_sync(comp.out());

	//	TYPES::Image image = new TYPES::Image(length);
	//	image->length(length);
	TYPES::ImageType_var image = TYPES::ImageType(length);
	image->length((CORBA::ULong)length);
	for(unsigned int i=0;i!=length;i++) {
		frame[0][i] = 0;
		image[i] = frame[0][i];
	}

	delete frame;

	ACS_SHORT_LOG((LM_INFO,"lpiImpl::image: Obtained the Image"));
	return image._retn();
}

void lpiImpl::lock() throw (CORBA::SystemException){
	m_locking = true;
}

void lpiImpl::unlock() throw (CORBA::SystemException){
	m_locking = false;
}

void lpiImpl::on() throw (CORBA::SystemException){
	m_powered = true;
}

void lpiImpl::off() throw (CORBA::SystemException){
	m_powered = false;
}

/* Attributes returning */
bool lpiImpl::locking() throw (CORBA::SystemException){
	return m_locking;
}

bool lpiImpl::powered() throw (CORBA::SystemException){
	return m_powered;
}

/* Properties returning */
ACS::RWstring_ptr lpiImpl::device() throw (CORBA::SystemException){
	if( m_device_sp == 0 ){
		return ACS::RWstring::_nil();
	}
	ACS::RWstring_var prop = ACS::RWstring::_narrow(m_device_sp->getCORBAReference());
	return prop._retn();
}

ACS::ROlongSeq_ptr lpiImpl::frame() throw (CORBA::SystemException){
	if( m_frame_sp == 0 ){
		return ACS::ROlongSeq::_nil();
	}
	ACS::ROlongSeq_var prop = ACS::ROlongSeq::_narrow(m_frame_sp->getCORBAReference());
	return prop._retn();
}

ACS::RWlong_ptr  lpiImpl::red() throw (CORBA::SystemException)
{
	if( m_red_sp == 0 ){
		return ACS::RWlong::_nil();
	}
	ACS::RWlong_var prop = ACS::RWlong::_narrow(m_red_sp->getCORBAReference());
	return prop._retn();
}

ACS::RWlong_ptr  lpiImpl::blue() throw (CORBA::SystemException)
{
	if( m_blue_sp == 0 ){
		return ACS::RWlong::_nil();
        }
        ACS::RWlong_var prop = ACS::RWlong::_narrow(m_blue_sp->getCORBAReference());
        return prop._retn();}

ACS::RWlong_ptr  lpiImpl::green() throw (CORBA::SystemException)
{
	if( m_green_sp == 0 ){
                return ACS::RWlong::_nil();
        }
        ACS::RWlong_var prop = ACS::RWlong::_narrow(m_green_sp->getCORBAReference());
        return prop._retn();}

ACS::RWlong_ptr  lpiImpl::pixelBias() throw (CORBA::SystemException)
{
	if( m_pixelBias_sp == 0 ){
                return ACS::RWlong::_nil();
        }
        ACS::RWlong_var prop = ACS::RWlong::_narrow(m_pixelBias_sp->getCORBAReference());
        return prop._retn();}

ACS::RWlong_ptr    lpiImpl::resetLevel() throw (CORBA::SystemException)
{
	if( m_resetLevel_sp == 0 ){
                return ACS::RWlong::_nil();
        }
        ACS::RWlong_var prop = ACS::RWlong::_narrow(m_resetLevel_sp->getCORBAReference());
        return prop._retn();}

ACS::RWlong_ptr    lpiImpl::exposure() throw (CORBA::SystemException)
{
	if( m_exposure_sp == 0 ){
                return ACS::RWlong::_nil();
        }
        ACS::RWlong_var prop = ACS::RWlong::_narrow(m_exposure_sp->getCORBAReference());
        return prop._retn();}


/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(lpiImpl)
/* ----------------------------------------------------------------*/

/*___oOo___*/
