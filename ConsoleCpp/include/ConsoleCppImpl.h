#include "acscomponentImpl.h"
#include "ConsoleS.h"
#include "SchedulerS.h"
#include "InstrumentS.h"
#include "TelescopeS.h"
#include "SYSTEMErr.h"
//#include "TypeS.h"

class ConsoleCppImpl :
	public acscomponent::ACSComponentImpl,
	public POA_CONSOLE_MODULE::Console

{

private:


	void execute();
	void cleanUp();

	SCHEDULER_MODULE::Scheduler_var scheduler_p;
	INSTRUMENT_MODULE::Instrument_var instrument_p;
	TELESCOPE_MODULE::Telescope_var telescope_p;
	bool m_mode;

public:

	
	ConsoleCppImpl(const ACE_CString &name, maci::ContainerServices *cs);
	~ConsoleCppImpl();
	
	void setMode(bool mode) throw (SYSTEMErr::AlreadyInAutomaticEx);
	bool getMode();

	void cameraOn() throw (SYSTEMErr::SystemInAutoModeEx);
	void cameraOff() throw (SYSTEMErr::SystemInAutoModeEx);

	void moveTelescope(const ::TYPES::Position & coordinates)
		throw (SYSTEMErr::PositionOutOfLimitsEx, SYSTEMErr::SystemInAutoModeEx);

	TYPES::Position getTelescopePosition();

	TYPES::ImageType* getCameraImage()
		throw (SYSTEMErr::SystemInAutoModeEx, SYSTEMErr::CameraIsOffEx);

	void setRGB(const ::TYPES::RGB & rgbConfig) throw (SYSTEMErr::CameraIsOffEx);
	void setPixelBias(::CORBA::Long bias) throw (SYSTEMErr::CameraIsOffEx);
	void setResetLevel(::CORBA::Long resetLevel) throw (SYSTEMErr::CameraIsOffEx);
};

