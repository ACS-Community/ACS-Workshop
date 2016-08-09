#ifndef TELESCOPE_h
#define TELESCOPE_h

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <TelescopeS.h>
#include <acscomponentImpl.h>
#include <ACSErrTypeCommon.h>
#include <TypesC.h>


class TelescopeImpl : public virtual POA_ACS::ACSComponent,
			public virtual POA_TELESCOPE_MODULE::Telescope

{
	public:
		// Lifecycle
		//
		TelescopeImpl(const ACE_CString& name,
                                            maci::ContainerServices* containerServices);

		virtual ~TelescopeImpl();
		
		virtual void initialize(void);

		virtual void execute(void);

		virtual void cleanUp(void);

		virtual void aboutToAbort(void);

		// End-Lifecycle

		virtual TYPES::ImageType* observe(const ::TYPES::Position & coordinates,
      ::CORBA::Long exposureTime);

		virtual void moveTo( const ::TYPES::Position & coordinates);

		virtual TYPES::Position getCurrentPosition();

};

#endif
