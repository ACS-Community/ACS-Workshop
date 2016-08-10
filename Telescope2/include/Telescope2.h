#ifndef TELESCOPE2_h
#define TELESCOPE2_h

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <TelescopeS.h>
#include <TelescopeControlC.h>
#include <InstrumentC.h>
#include <acscomponentImpl.h>
#include <TypesC.h>
#include <SYSTEMErr.h>

class TelescopeImpl : public virtual acscomponent::ACSComponentImpl,
			public virtual POA_TELESCOPE_MODULE::Telescope

{
	public:
		// Lifecycle
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

	private:	
		TELESCOPE_MODULE::TelescopeControl_var mount;
		INSTRUMENT_MODULE::Instrument_var instrument;

};

#endif
