#ifndef _CAMERA_IMPL_H_
#define _CAMERA_IMPL_H_

#include <baciCharacteristicComponentImpl.h>
#include <baciSmartPropertyPointer.h>
#include <baciRWstring.h>

#include <CameraS.h>


using namespace baci;

class CameraImpl: public virtual CharacteristicComponentImpl,
	public virtual POA_CAMERA_MODULE::Camera
{
	public:
	CameraImpl(const ACE_CString& name, maci::ContainerServices* containerServices);
	virtual ~CameraImpl();
	virtual ::TYPES::ImageType * takeImage (const char * exposureTime, const char * iso);
	virtual ACS::RWstring_ptr shutterSpeed() throw(CORBA::SystemException);
	virtual ACS::RWstring_ptr isoSpeed() throw(CORBA::SystemException);

	/* ------------------- [ Lifecycle START interface ] --------------------- */

	/**
	 *                  * Lifecycle method called at component initialization state.
	 *                                   * Overwrite it in case you need it.
	 *                                                    */
	virtual void initialize(void) throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl);

	/**
	 *                  * Lifecycle method called at component execute state.
	 *                                   * Overwrite it in case you need it.
	 *                                                    */
	virtual void execute(void) throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl);

	/**
	 *                  * Lifecycle method called at component clean up state.
	 *                                   * Overwrite it in case you need it.
	 *                                                    */
	virtual void cleanUp(void);

	/**
	 *            * Lifecycle method called at component abort state.
	 *                       * Overwrite it in case you need it.
	 *                                  */
	virtual void aboutToAbort(void);

	/* ------------------- [ Lifecycle END interface ] --------------- */

	private:
	CameraImpl(const CameraImpl&);
	void operator=(const CameraImpl&);

	SmartPropertyPointer<RWstring> shutterSpeed_m;
	SmartPropertyPointer<RWstring> iso_m;

	std::string  component_name;

};

#endif
