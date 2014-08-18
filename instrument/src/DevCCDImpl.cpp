#include <baciDB.h>
#include <ACSErrTypeCommon.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <DevCCDImpl.h>

#include <linux/videodev2.h>

#include <lpiPropsDevIO.h>
#include <lpiFrameDevIO.h>

#include <acserr.h>

ACE_RCSID(instrument, DevCCDImpl, "$Id: DevCCDImpl.cpp,v 1.9 2010-11-19 18:06:14 instrument Exp $")
using namespace baci;


DevCCDImpl::DevCCDImpl(
             ACE_CString _name,
             maci::ContainerServices * containerServices) :
    CharacteristicComponentImpl(_name, containerServices)
    
    
    ,
    m_frame_sp(this),
    m_device_sp(this),
    m_red_sp(this),
    m_blue_sp(this),
    m_green_sp(this),
    m_pixelBias_sp(this),
    m_resetLevel_sp(this),
    m_exposure_sp(this)

    
    
{
    ACS_TRACE("::Instrument::DevCCD Component");

/*
    m_frame_sp = new ROlongSeq(_name+":frame", getComponent());
    m_device_sp=new RWstring(_name+":device", getComponent());
    m_red_sp=new RWlong(_name+":red", getComponent());
    m_blue_sp=new RWlong(_name+":blue", getComponent());
    m_green_sp=new RWlong(_name+":green", getComponent());
    m_pixelBias_sp=new RWlong(_name+":pixelBias", getComponent());
    m_resetLevel_sp=new RWlong(_name+":resetLevel", getComponent());
    m_exposure_sp=new RWlong(_name+":exposure", getComponent());
*/
    
    video_fd = 0;
    video_fd = open("/dev/video1", O_RDWR);

    if (video_fd <= 0) {
        ACS_SHORT_LOG((LM_CRITICAL, "WRONG video file descriptor"));
    } else {
        ACS_SHORT_LOG((LM_INFO, "Video file descriptor obtained"));
    } 
   	m_frame_sp = new ROlongSeq(_name+":frame", getComponent(),new lpiFrameDevIO(video_fd, "/dev/video0"), true);
	m_red_sp = new RWlong(_name+":red", getComponent(),new lpiPropsDevIO( video_fd, V4L2_CID_RED_BALANCE ));
	m_blue_sp = new RWlong(_name+":blue", getComponent(),new lpiPropsDevIO( video_fd, V4L2_CID_BLUE_BALANCE));
	m_green_sp = new RWlong(_name+":green", getComponent(),new lpiPropsDevIO( video_fd, SN9C102_V4L2_CID_GREEN_BALANCE ));
	m_pixelBias_sp = new RWlong(_name+":pixelBias", getComponent(),new lpiPropsDevIO( video_fd, SN9C102_V4L2_CID_PIXEL_BIAS_VOLTAGE ));
	m_resetLevel_sp = new RWlong(_name+":resetLevel", getComponent(),new lpiPropsDevIO( video_fd, SN9C102_V4L2_CID_RESET_LEVEL ));

    m_device_sp=new RWstring(_name+":device", getComponent());
    m_exposure_sp=new RWlong(_name+":exposure", getComponent());
    
}


void DevCCDImpl::on()
{
   ACS_TRACE("::Instrument:: Turning Camera On");

   video_fd = open("/dev/video0", O_RDWR);
    
}

void DevCCDImpl::off()
{
   ACS_TRACE("::Instrument:: Turning Camera Off");
   close(video_fd);
}



ACS::ROlongSeq_ptr
DevCCDImpl::frame ()
{
    if (m_frame_sp == 0)
	{
	return ACS::ROlongSeq::_nil();
	}

    ACS::ROlongSeq_var prop = ACS::ROlongSeq::_narrow(m_frame_sp->getCORBAReference());
    
    return prop._retn();
}

TYPES::ImageType* DevCCDImpl::image(CORBA::Double expo) {
    
    /*if (video_fd == 0) {
        throw();
    }*/
    
    ACSErr::Completion_var completion;

	//ACS::longSeq_var data = m_frame_sp->get_sync(completion.out()); problem with the camera
	
	TYPES::ImageType_var myImage = new TYPES::ImageType( (CORBA::ULong)(640*480*3));
	
	myImage->length((CORBA::ULong)(640*480*3));
	
	for( int i = 0; i < (640*480*3); i++)
		myImage[i] = 0;

	return myImage._retn();
   
}

ACS::RWstring_ptr
DevCCDImpl::device ()
{
    if (m_device_sp == 0)
	{
	return ACS::RWstring::_nil();
	}

    ACS::RWstring_var prop = ACS::RWstring::_narrow(m_device_sp->getCORBAReference());
    return prop._retn();
}


ACS::RWlong_ptr
DevCCDImpl::red ()
{
    if (m_red_sp == 0)
	{
	return ACS::RWlong::_nil();
	}

    ACS::RWlong_var prop = ACS::RWlong::_narrow(m_red_sp->getCORBAReference());
    return prop._retn();
}


ACS::RWlong_ptr
DevCCDImpl::blue ()
{
    if (m_blue_sp == 0)
	{
	return ACS::RWlong::_nil();
	}

    ACS::RWlong_var prop = ACS::RWlong::_narrow(m_blue_sp->getCORBAReference());
    return prop._retn();
}

ACS::RWlong_ptr
DevCCDImpl::green ()
{
    if (m_green_sp == 0)
	{
	return ACS::RWlong::_nil();
	}

    ACS::RWlong_var prop = ACS::RWlong::_narrow(m_green_sp->getCORBAReference());
    return prop._retn();
}

ACS::RWlong_ptr
DevCCDImpl::pixelBias ()
{
    if (m_pixelBias_sp == 0)
	{
	return ACS::RWlong::_nil();
	}

    ACS::RWlong_var prop = ACS::RWlong::_narrow(m_pixelBias_sp->getCORBAReference());
    return prop._retn();
}

ACS::RWlong_ptr
DevCCDImpl::resetLevel ()
{
    if (m_resetLevel_sp == 0)
	{
	return ACS::RWlong::_nil();
	}

    ACS::RWlong_var prop = ACS::RWlong::_narrow(m_resetLevel_sp->getCORBAReference());
    return prop._retn();
}

ACS::RWlong_ptr
DevCCDImpl::exposure ()
{
    if (m_exposure_sp == 0)
	{
	return ACS::RWlong::_nil();
	}

    ACS::RWlong_var prop = ACS::RWlong::_narrow(m_exposure_sp->getCORBAReference());
    return prop._retn();
}

/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(DevCCDImpl)
/* ----------------------------------------------------------------*/
