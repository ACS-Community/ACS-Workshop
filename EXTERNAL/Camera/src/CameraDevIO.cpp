#include "CameraDevIO.h"
#include <capture.h>
#include <gphoto2/gphoto2-camera.h>

ShutterSpeedDevIO::ShutterSpeedDevIO()
{

}


ShutterSpeedDevIO::ShutterSpeedDevIO(void *data) 
{

}

ShutterSpeedDevIO::~ShutterSpeedDevIO()
{
}

bool ShutterSpeedDevIO::initializeValue()
{
	return true;
}

ACE_CString ShutterSpeedDevIO::read(ACS::Time& timestamp)
	throw (ACSErr::ACSbaseExImpl)
{
	Camera *camera;
	GPContext *context;
	gp_camera_new(&camera);
    	context = gp_context_new();
	
	Capture capture = Capture (camera, context);

	const ACE_CString value = capture.getShutterSpeed(camera, context).c_str();
	return value;
}

void ShutterSpeedDevIO::write(const ACE_CString& value, ACS::Time& timestamp)
	throw (ACSErr::ACSbaseExImpl)
{
	Camera *camera;
	GPContext *context;
	gp_camera_new(&camera);
    	context = gp_context_new();
	Capture capture = Capture (camera, context);

	capture.setShutterSpeed(camera, context, value.c_str());
}

IsoDevIO::IsoDevIO()
{
}


IsoDevIO::IsoDevIO(void *data) 
{
}

IsoDevIO::~IsoDevIO()
{
}

bool IsoDevIO::initializeValue()
{
	return true;
}

ACE_CString IsoDevIO::read(ACS::Time& timestamp)
	throw (ACSErr::ACSbaseExImpl)
{
	Camera *camera;
	GPContext *context;
	gp_camera_new(&camera);
    	context = gp_context_new();
	Capture capture = Capture (camera, context);

	const ACE_CString value = capture.getIso(camera, context).c_str();
	return value;
	
}

void IsoDevIO::write(const ACE_CString& value, ACS::Time& timestamp)
	throw (ACSErr::ACSbaseExImpl)
{
	Camera *camera;
	GPContext *context;
	gp_camera_new(&camera);
    	context = gp_context_new();
	Capture capture = Capture (camera, context);

	capture.setIso(camera, context, value.c_str());
}
