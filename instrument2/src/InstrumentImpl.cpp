#include <InstrumentImpl.h>
#include <ACSErrTypeCommon.h>
#include <iostream>


InstrumentImpl::InstrumentImpl(
                       const ACE_CString &name,
                       maci::ContainerServices * containerServices) :
    ACSComponentImpl(name, containerServices)
{
	ACS_TRACE("InstrumentImpl::InstrumentImpl()");
}

InstrumentImpl::~InstrumentImpl()
{
	ACS_TRACE("InstrumentImpl::~InstrumentImpl()");
}

void InstrumentImpl::cameraOn ()
{
	encendido = true;
	cout << "Camara Encendida :)" << endl;
	ACS_TRACE("InstrumentImpl::cameraOn()");
}

void InstrumentImpl::cameraOff ()
{
	encendido = false;
	cout << "Camara Apagada :(" << endl;
	ACS_TRACE("InstrumentImpl::cameraOff()");
}

::CORBA::Long InstrumentImpl::takeImage ()
{
	cout << "Sonria! :P" << endl;
	ACS_TRACE("InstrumentImpl::takeImage()");
	return (long)0;
}

void InstrumentImpl::setRGB ()
{
	ACS_TRACE("InstrumentImpl::setRGB()");
	cout << "Colores Listos" << endl;
}

void InstrumentImpl::setPixelBias ()
{
	ACS_TRACE("InstrumentImpl::setPixelBias()");
	cout << "Sesgo de Colores corregido" << endl;
}

void InstrumentImpl::setResetLevel ()
{
	ACS_TRACE("InstrumentImpl::setResetLevel()");
	cout << "Camara reseteada" << endl;
}


/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(InstrumentImpl)
/* ----------------------------------------------------------------*/

