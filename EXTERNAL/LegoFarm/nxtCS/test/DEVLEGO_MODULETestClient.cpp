/*
 * "@(#) $Id: DEVLEGO_MODULETestClient.cpp,v 1.1 2009/11/09 03:01:39 almamgr Exp $"
 *
 * $Log: DEVLEGO_MODULETestClient.cpp,v $
 * Revision 1.1  2009/11/09 03:01:39  almamgr
 * Initial commit for legacy software: LegoFarm (jslopez)
 *
 *
 */

#include <acsutil.h>
#include <maciSimpleClient.h>
#include <baciC.h>
#include <logging.h>

#include <DevLegoC.h>

using namespace maci;


char* cobname;

int main(int argc, char* argv[])
{
	// Check command line arguments
	if (argc < 2)
	{
		cobname = "DevLego";
	}
	else
	{
		cobname = argv[1];
	}

	// Creates and initializes the SimpleClient object
	SimpleClient client;

	if(client.init(argc,argv) == 0)
	{
		ACS_SHORT_LOG((LM_ERROR,"Cannot init client"));
		return -1;
	}

	ACS_SHORT_LOG((LM_INFO, "Welcome to DEVLEGO_MODULETestClient!"));
	ACS_SHORT_LOG((LM_INFO, "Login in maciManager..."));
	client.login();

	try
	{
		// List all components the Manager knows of our type.
		ACS_SHORT_LOG((LM_INFO, "Listing all componentes of type *DevLego*"));
		maci::HandleSeq seq;
		maci::ComponentInfoSeq_var components = 
			       client.manager()->get_component_info(client.handle(), seq, "*", 
								                                  "*DevLego*", false);

		for (CORBA::ULong i = 0; i < components->length(); i++)
		{
			ACS_SHORT_LOG((LM_INFO,"%s (%s)", components[i].name.in(), 
						                            components[i].type.in()));
		}

		// Now get the specific component we have requested on the command line.
		ACS_SHORT_LOG((LM_INFO, "Getting component DevLego..."));
		DEVLEGO_MODULE::DevLego_var component = 
			       client.get_object<DEVLEGO_MODULE::DevLego>("DevLego", 0, true);

		if (!CORBA::is_nil(component.in()))
		{
			ACS_SHORT_LOG((LM_INFO, "... got component DevLego"));
			// Prints the descriptor of the requested component.
			ACS_SHORT_LOG((LM_INFO, "Requesting descriptor()... "));
			ACS::CharacteristicComponentDesc_var descriptor = 
				                                       component->descriptor();

			ACS_SHORT_LOG((LM_INFO, "Descriptor:"));
			ACS_SHORT_LOG((LM_INFO, "\tname: %s", descriptor->name.in()));

			ACSErr::Completion_var completion;


			ACS_SHORT_LOG((LM_INFO, "Getting component property: %s:status", 
						                  cobname));
			if(CORBA::is_nil(component->status()) == false)
			{ 
				ACS_SHORT_LOG((LM_INFO, "No auto generated return type in the "
							                  "acsGenerator found! Ignoring property."));
			}


			ACS_SHORT_LOG((LM_INFO, "Getting component property: %s::posAlt", 
						                                                       cobname));
			if (CORBA::is_nil(component->posAlt()) == false)
			{ 
				CORBA::Double value = component->posAlt()->get_sync(completion.out());
				ACS_SHORT_LOG((LM_INFO, "Value: %f", value));
			}


			ACS_SHORT_LOG((LM_INFO, "Getting component property: %s::posAzm", 
						                                                       cobname));
			if (CORBA::is_nil(component->posAzm()) == false)
			{ 
				CORBA::Double value = component->posAzm()->get_sync(completion.out());
				ACS_SHORT_LOG((LM_INFO, "Value: %f", value));
			}


			ACS_SHORT_LOG((LM_INFO, "Getting component property: %s::velAlt", 
						                                                       cobname));
			if (CORBA::is_nil(component->velAlt()) == false)
			{ 
				CORBA::Double value = component->velAlt()->get_sync(completion.out());
				ACS_SHORT_LOG((LM_INFO, "Value: %f", value));
			}


			ACS_SHORT_LOG((LM_INFO, "Getting component property: %s::velAzm", 
						                                                       cobname));
			if (CORBA::is_nil(component->velAzm()) == false)
			{ 
				CORBA::Double value = component->velAzm()->get_sync(completion.out());
				ACS_SHORT_LOG((LM_INFO, "Value: %f", value));
			}


			ACS_SHORT_LOG((LM_INFO, "Getting component property: %s::posX", 
						                                                       cobname));
			if (CORBA::is_nil(component->posX()) == false)
			{ 
				CORBA::Double value = component->posX()->get_sync(completion.out());
				ACS_SHORT_LOG((LM_INFO, "Value: %f", value));
			}


			ACS_SHORT_LOG((LM_INFO, "Getting component property: %s::posY", 
						                                                       cobname));
			if (CORBA::is_nil(component->posY()) == false)
			{ 
				CORBA::Double value = component->posY()->get_sync(completion.out());
				ACS_SHORT_LOG((LM_INFO, "Value: %f", value));
			}


			ACS_SHORT_LOG((LM_INFO, "Getting component property: %s::velX", 
						                                                       cobname));
			if (CORBA::is_nil(component->velX()) == false)
			{ 
				CORBA::Double value = component->velX()->get_sync(completion.out());
				ACS_SHORT_LOG((LM_INFO, "Value: %f", value));
			}


			ACS_SHORT_LOG((LM_INFO, "Getting component property: %s::velY", 
						                                                       cobname));
			if (CORBA::is_nil(component->velY()) == false)
			{ 
				CORBA::Double value = component->velY()->get_sync(completion.out());
				ACS_SHORT_LOG((LM_INFO, "Value: %f", value));
			}



		} /* end if component reference OK */
		else
		{
			ACS_SHORT_LOG((LM_INFO, "Component DevLego is nil !!!"));
		}
	} /* end main try */
	catch (...)
	{
		ACS_SHORT_LOG((LM_ERROR, "Error in TestClient::main!"));
	}

	/* Another try section where we release our component
	 * and logout from the Manager.
	 */
	try
	{
		ACS_SHORT_LOG((LM_INFO, "Releasing..."));
		client.manager()->release_component(client.handle(), cobname);
		client.logout();
	}
	catch (...)
	{
		ACS_SHORT_LOG((LM_ERROR, "Error in TestClient::main!"));
	}

	/*
	 * sleep for 3 sec to allow everytihng to cleanup and stabilize
	 * so that the tests can be determinitstic.
	 */
	ACE_OS::sleep(3);
	return 0;
}
