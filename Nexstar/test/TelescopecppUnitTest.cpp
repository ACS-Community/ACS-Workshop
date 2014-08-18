#include <cppunit/TestSuite.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <math.h>

#include <maciSimpleClient.h>

#include <TelescopeC.h> 

maci::SimpleClient client;

class TelescopeTest: public CppUnit::TestFixture {
	private:
		TELESCOPE_MODULE::Telescope *comp;
	public:
	
		void setUp() {
			comp = client.getComponent<TELESCOPE_MODULE::Telescope>("TELESCOPE", 0, true);
		}

		void runTest() {
		  TYPES::Position targetPos;
		  TYPES::Position realPos;

		  targetPos.az  = 10;
		  targetPos.el = 11;

		  comp->moveTo(targetPos);
                  realPos = comp->getCurrentPosition();
		  CPPUNIT_ASSERT(fabsl(targetPos.el-realPos.el)>5 || fabsl(targetPos.az-realPos.az)>5);
		}

		void tearDown(){
			client.releaseComponent("TELESCOPE");
		}

  CPPUNIT_TEST_SUITE(TelescopeTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();
};


int main(int argc, char *argv[]) {
	client.init(argc, argv);
	client.login();

	CppUnit::TextUi::TestRunner runner;
	runner.addTest(TelescopeTest::suite());
	runner.run();

	client.logout();
}
