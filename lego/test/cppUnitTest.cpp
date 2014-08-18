#include <cppunit/TestSuite.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

#include <maciSimpleClient.h>

#include "TelescopeC.h"
#include "TypesC.h"

maci::SimpleClient client;

class legoImplTest: public CppUnit::TestFixture {
	private:
		TELESCOPE_MODULE::Telescope *comp;
	public:
	
		void setUp() {
			comp = client.getComponent<TELESCOPE_MODULE::Telescope>("lego", 0, true);
		}

		void runTest() {
			TYPES::Position test;
			TYPES::Position final;
			test.az = 20;
			test.el = 10;
 
			comp->moveTo(test);
			
			final = comp->getCurrentPosition();
			CPPUNIT_ASSERT(test.az == final.az);
			CPPUNIT_ASSERT(test.el == final.el);
		}

		void tearDown(){
			client.releaseComponent("lego");
		}


 CPPUNIT_TEST_SUITE(legoImplTest);
 CPPUNIT_TEST(runTest);
 CPPUNIT_TEST_SUITE_END();
};


int main(int argc, char *argv[]) {
	client.init(argc, argv);
	client.login();

	CppUnit::TextUi::TestRunner runner;
	runner.addTest(legoImplTest::suite());
	runner.run();

	client.logout();
}
