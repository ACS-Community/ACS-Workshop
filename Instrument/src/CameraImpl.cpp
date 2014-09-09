#include <CameraImpl.h>



CameraImpl::CameraImpl(
	     const ACE_CString &name,
	     maci::ContainerServices *& containerServices):
	acscomponent::ACSComponentImpl(name, containerServices)
{


}

CameraImpl::~CameraImpl()
{

}


TYPES::ImageType* CameraImpl::getFrame(CORBA::Long exposure)
{
   TYPES::ImageType_var data(new TYPES::ImageType);
   data->length(500);
   for(int i=0;i<500;i++)
     data[i]=exposure;
   return data._retn();
}

