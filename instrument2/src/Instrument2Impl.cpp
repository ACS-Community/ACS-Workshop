#include <Instrument2Impl.h>
#include <ACSErrTypeCommon.h>
#include <iostream>


Instrument2Impl::Instrument2Impl(
                       const ACE_CString &name,
                       maci::ContainerServices * containerServices) :
    ACSComponentImpl(name, containerServices)
{
	ACS_TRACE("Instrument2Impl::Instrument2Impl()");
	Instrument_ref = INSTRUMENT_MODULE::Instrument::_nil();

}

Instrument2Impl::~Instrument2Impl()
{
	ACS_TRACE("Instrument2Impl::~Instrument2Impl()");
}

void Instrument2Impl::cameraOn ()
{
	encendido = true;
	cout << "Camara Encendida :)" << endl;
	ACS_TRACE("Instrument2Impl::cameraOn()");

	Instrument_ref = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT2");

	if (CORBA::is_nil(Instrument_ref.in()))
	{
		printf("\nCORBA::is_nil(Instrument_ref.in()\n" );
		ACS_TRACE("Instrument_ref = NULL");
	}
	else
	{
		Instrument_ref->cameraOn();
		cout << "INSTRUMENT2: Instrument_ref->cameraOn()" << endl;
		ACS_TRACE("Instrument_ref->cameraOn()");
	}
	
	
}

void Instrument2Impl::cameraOff ()
{
	encendido = false;
	cout << "Camara Apagada :(" << endl;
	ACS_TRACE("Instrument2Impl::cameraOff()");

	Instrument_ref = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT2");

	if (CORBA::is_nil(Instrument_ref.in()))
	{
		printf("\nCORBA::is_nil(Instrument_ref.in()\n" );
		ACS_TRACE("Instrument_ref = NULL");
	}
	else
	{
		Instrument_ref->cameraOff();
		cout << "INSTRUMENT2: Instrument_ref->cameraOff()" << endl;
		ACS_TRACE("Instrument_ref->cameraOff()");
	}
}

::CORBA::Long Instrument2Impl::takeImage ()
{
	cout << "Sonria! :P" << endl;
	ACS_TRACE("Instrument2Impl::takeImage()");
	
	::CORBA::Long var;
	Instrument_ref = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT2");

	if (CORBA::is_nil(Instrument_ref.in()))
	{
		printf("\nCORBA::is_nil(Instrument_ref.in()\n" );
		ACS_TRACE("Instrument_ref = NULL");
	}
	else
	{
		var = Instrument_ref->takeImage();
		cout << "INSTRUMENT2: Instrument_ref->takeImage() = " << var << endl;
		ACS_TRACE("Instrument_ref->takeImage()");
		return var;
	}

	//if (CORBA::is_nil(Instrument_ref.in()) == false)

	return 666;
}

void Instrument2Impl::setRGB ()
{
	ACS_TRACE("Instrument2Impl::setRGB()");
	cout << "Colores Listos" << endl;

	Instrument_ref = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT2");

	if (CORBA::is_nil(Instrument_ref.in()))
	{
		printf("\nCORBA::is_nil(Instrument_ref.in()\n" );
		ACS_TRACE("Instrument_ref = NULL");
	}
	else
	{
		Instrument_ref->setRGB();
		cout << "Instrument_ref->setRGB()" << endl;
		ACS_TRACE("Instrument_ref->setRGB()");
	}
}

void Instrument2Impl::setPixelBias ()
{
	ACS_TRACE("Instrument2Impl::setPixelBias()");
	cout << "Sesgo de Colores corregido" << endl;

	Instrument_ref = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT2");

	if (CORBA::is_nil(Instrument_ref.in()))
	{
		printf("\nCORBA::is_nil(Instrument_ref.in()\n" );
		ACS_TRACE("Instrument_ref = NULL");
	}
	else
	{
		Instrument_ref->setPixelBias();
		cout << "Instrument_ref->setPixelBias()" << endl;
		ACS_TRACE("Instrument_ref->setPixelBias()");
	}
}

void Instrument2Impl::setResetLevel ()
{
	ACS_TRACE("Instrument2Impl::setResetLevel()");
	cout << "Camara reseteada" << endl;

	Instrument_ref = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT2");

	if (CORBA::is_nil(Instrument_ref.in()))
	{
		printf("\nCORBA::is_nil(Instrument_ref.in()\n" );
		ACS_TRACE("Instrument_ref = NULL");
	}
	else
	{
		Instrument_ref->setResetLevel();
		cout << "Instrument_ref->setResetLevel()" << endl;
		ACS_TRACE("Instrument_ref->setResetLevel)");
	}
}


/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(Instrument2Impl)
/* ----------------------------------------------------------------*/

