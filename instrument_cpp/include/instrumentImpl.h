#include "acscomponentImpl.h"
#include "instrumentS.h"

class instrumentImpl : 
	public acscomponent::ACSComponentImpl,
	public POA_instrumentNS::instrument {
public:       
        instrumentImpl(const ACE_Cstring &name, maci::ContainerSrvices *cs);
	~instrumentImpl();
// lifecycle methods
        char* sayHello(); // implementation of IDL method
        }
