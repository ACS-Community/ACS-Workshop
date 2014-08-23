#include <cppunit/TestFixture.h>
#include <cppunit/TestCaller.h>
#include <maciSimpleClient.h>

#include "HelloDemoC.h"

maci::SimpleClient client;

class HelloDemoTest: public CppUnit::TestFixture {
	private:
		demo::HelloDemo *comp;
	public:
	
		void setUp() {
			comp = client.getComponent<demo::HelloDemo>("COMPONENT_NAME", 0, true);
		}

		void runTest() {
			char * res = comp->sayHello();
			CPPUNIT_ASSERT(res != NULL);
		}

		void tearDown(){
			client.releaseComponent("COMPONENT_NAME");
		}
};


int main(int argc, char *argv[]) {
	client.init(argc, argv);
	client.login();

	CppUnit::TestCaller<HelloDemoTest> test ("testComponent", 
			&HelloDemoTest::runTest);

	client.logout();
}
