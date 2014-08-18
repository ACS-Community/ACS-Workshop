#include <maciSimpleClient.h>
#include <AltAzC.h>
#include "acsexmplCallbacks.h"

ACE_RCSID(altaz, altazClient, "$Id: AltAzClient.cpp,v 1.1 2010-11-15 21:54:11 scheduler Exp $")
using namespace maci;
/*******************************************************************************/
int main(int argc, char *argv[])
{   
   //Checks command-line arguments.
   if (argc < 2)
   {
      ACS_SHORT_LOG((LM_INFO, "Usage: %s <component name> <options>", argv[0]));
      return -1;
   }
   else
   {
      ACS_SHORT_LOG((LM_INFO, "Welcome to %s!", argv[0]));
   }

   //Creates and initializes the SimpleClient object
   SimpleClient client;
   if (client.init(argc,argv) == 0)
   {
      ACE_DEBUG((LM_DEBUG,"Cannot init client"));
      return -1;
   }
   else
   {
      //Must log into manager before we can really do anything
      client.login();
   }

   try
   {
      //Get the specific component we have requested on the command-line
      AltAzModule::AltAz_var altaz = client.get_object<AltAzModule::AltAz>(argv[1], 0, true);
      MyCBvoid myCallback("moveto");

      ACS::CBvoid_var cb = myCallback._this();	
      ACS::CBDescIn desc;
      desc.id_tag = 2;

      //Run whatever supported command the end-user has specified
      //from the command-line

      altaz->moveto(cb.in(),desc,(double)atoi(argv[2]),(double)atoi(argv[3]));
      ACS_SHORT_LOG((LM_INFO,"Moving to %lf %lf ...",(double)atoi(argv[2]),(double)atoi(argv[3])));
      ACE_OS::sleep(5);
      altaz->moveto(cb.in(),desc,0,0);
      ACS_SHORT_LOG((LM_INFO,"Moving to 0 0 ..."));
   }
   catch(...)
   {
      ACS_SHORT_LOG((LM_ERROR,"main"));
   }

   try
   {
      //Release the component and log out from manager.
      ACS_SHORT_LOG((LM_INFO,"Releasing..."));
      client.manager()->release_component(client.handle(), argv[1]);
      client.logout();
   }
   catch(...)
   {
      ACS_SHORT_LOG((LM_ERROR, "main"));
   }

   // sleep for 3 sec.
   ACE_OS::sleep(3);
   return 0;
}

/*___oOo___*/



