#ifndef _CAMERA_DEV_IO_H_
#define _CAMERA_DEV_IO_H_

#include <baciDevIO.h>

class ShutterSpeedDevIO: public DevIO<ACE_CString>
{
	public:
	ShutterSpeedDevIO();
	ShutterSpeedDevIO(void *data);
	virtual ~ShutterSpeedDevIO();
	
	virtual bool initializeValue();
	ACE_CString read(ACS::Time& timestamp) throw (ACSErr::ACSbaseExImpl);
	void write(const ACE_CString& value, ACS::Time& timestamp) throw (ACSErr::ACSbaseExImpl);
};


class IsoDevIO: public DevIO<ACE_CString>
{
        public:
        IsoDevIO();
        IsoDevIO(void *data);
        virtual ~IsoDevIO();

        virtual bool initializeValue();
        ACE_CString read(ACS::Time& timestamp) throw (ACSErr::ACSbaseExImpl);
        void write(const ACE_CString& value, ACS::Time& timestamp) throw (ACSErr::ACSbaseExImpl);
};


#endif

