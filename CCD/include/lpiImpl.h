#ifndef _LIPIMPL_H_
#define _LIPIMPL_H_

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#define SN9C102_V4L2_CID_GREEN_BALANCE (V4L2_CID_PRIVATE_BASE + 1)
#define SN9C102_V4L2_CID_RESET_LEVEL (V4L2_CID_PRIVATE_BASE + 2)
#define SN9C102_V4L2_CID_PIXEL_BIAS_VOLTAGE (V4L2_CID_PRIVATE_BASE + 3)

#include <baciCharacteristicComponentImpl.h>
#include <baciSmartPropertyPointer.h>

#include <baciROlongSeq.h>
#include <baciRWstring.h>
#include <baciRWlong.h>
#include <baciRWdouble.h>

#include <DevCCDS.h>
#include <TypesC.h>

using namespace baci;

/**
 * The class DevTelescope implements 
 */

class lpiImpl : public virtual CharacteristicComponentImpl,
                public virtual POA_DEVCCD_MODULE::DevCCD
{

public:

	lpiImpl(const ACE_CString& name, maci::ContainerServices* containerServices);

	virtual void aboutToAbort();

	virtual ~lpiImpl();

	virtual void initialize() throw (acsErrTypeLifeCycle::LifeCycleExImpl);

	virtual TYPES::ImageType* image(CORBA::Double exposure) throw (CORBA::SystemException);

	virtual void on() throw (CORBA::SystemException);

	virtual void off() throw (CORBA::SystemException);
 
	virtual void lock() throw (CORBA::SystemException);

	virtual void unlock() throw (CORBA::SystemException);

	virtual bool locking() throw (CORBA::SystemException);

	virtual bool powered() throw (CORBA::SystemException);

	virtual ACS::ROlongSeq_ptr frame()  throw (CORBA::SystemException);

	virtual ACS::RWstring_ptr  device() throw (CORBA::SystemException);

	virtual ACS::RWlong_ptr  red() throw (CORBA::SystemException);

	virtual ACS::RWlong_ptr  blue() throw (CORBA::SystemException);

	virtual ACS::RWlong_ptr  green() throw (CORBA::SystemException);

	virtual ACS::RWlong_ptr  pixelBias() throw (CORBA::SystemException);

	virtual ACS::RWlong_ptr    resetLevel() throw (CORBA::SystemException);

	virtual ACS::RWlong_ptr    exposure() throw (CORBA::SystemException);


private:

	// Properties
	SmartPropertyPointer<ROlongSeq> m_frame_sp;
	SmartPropertyPointer<RWstring>  m_device_sp;
	SmartPropertyPointer<RWlong>    m_red_sp;
	SmartPropertyPointer<RWlong>    m_blue_sp;
	SmartPropertyPointer<RWlong>    m_green_sp;
	SmartPropertyPointer<RWlong>    m_pixelBias_sp;
	SmartPropertyPointer<RWlong>    m_resetLevel_sp;
	SmartPropertyPointer<RWlong>    m_exposure_sp;

	// Private attributes (IDL)
	bool m_locking;
	bool m_powered;

	// Private attributes (others)
	std::string  component_name;
	int fd;

	/**
	 * ALMA C++ coding standards state copy operators should be disabled.
	 */
	void operator=(const lpiImpl&);
};

#endif /* _LIPIMPL_H_ */
