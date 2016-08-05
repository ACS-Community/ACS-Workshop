#include "CameraImpl.h"
#include "CameraDevIO.h"
#include <capture.h>
#include <gphoto2/gphoto2-camera.h>



CameraImpl::CameraImpl(const ACE_CString& name, maci::ContainerServices* containerServices) :
	 CharacteristicComponentImpl(name, containerServices),
	 shutterSpeed_m(this),
	 iso_m(this)
{
	component_name = name.c_str();
}

CameraImpl::~CameraImpl()
{

}

void CameraImpl::initialize(void)
	throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl)
{
	shutterSpeed_m = new RWstring((component_name + ":shutterSpeed").c_str(),
			getComponent()/*, new ShutterSpeedDevIO*/);
	iso_m =  new RWstring((component_name + ":isoSpeed").c_str(),
			getComponent()/*, new IsoDevIO*/);

}

void CameraImpl::execute(void)
	throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl)
{
}

void CameraImpl::cleanUp(void)
{
}

void CameraImpl::aboutToAbort(void)
{
}

TYPES::ImageType * CameraImpl::takeImage (const char * exposureTime, const char * iso)
{
	::Camera *camera;
	::GPContext *context;

	gp_camera_new(&camera);
	context = gp_context_new();
	Capture capture = Capture (camera, context);
	std::string filename = "image.tmp.jpg";
	
	capture.setShutterSpeed(camera, context, exposureTime);
	capture.setIso(camera, context, iso);
	
	int fileDesc = capture.capturePicture(camera, context,filename);
	close(fileDesc);
	gp_camera_unref(camera);
        gp_context_unref(context);

	fileDesc =  open(filename.c_str(), O_RDONLY);
	char* buffer;
	buffer = (char *)malloc(sizeof(char)*1024*1024*10);
	size_t bytes_read;
	bytes_read = read (fileDesc, buffer, sizeof(char)*1024*1024*10);
	close(fileDesc);
	::TYPES::ImageType_var ret_val = ::TYPES::ImageType(0);
	if (bytes_read < 0) {
		ret_val->length(0);
		return ret_val._retn();
	}
	ret_val->length(bytes_read);
	for (int i = 0; i < bytes_read; i++) {
		ret_val[i]=buffer[i];
	}
	return ret_val._retn();
}

ACS::RWstring_ptr CameraImpl::shutterSpeed() throw(CORBA::SystemException)
{
	if (shutterSpeed_m == 0)
		return ACS::RWstring::_nil();
	ACS::RWstring_var prop = ACS::RWstring::_narrow(shutterSpeed_m->getCORBAReference());
	return prop._retn();
}

ACS::RWstring_ptr CameraImpl::isoSpeed() throw(CORBA::SystemException)
{
	if (iso_m == 0)
		return ACS::RWstring::_nil();
	ACS::RWstring_var prop = ACS::RWstring::_narrow(iso_m->getCORBAReference());
	return prop._retn();
}

/* --------------- [ MACI DLL support functions ] -----------------*/

#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(CameraImpl)
