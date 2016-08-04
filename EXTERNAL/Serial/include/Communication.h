#include <SerialRS232.h>

using namespace std;

/** Movement in the Altitud axis in positive direction */
#define ALT_POS   0x1

/** Movement in the Altitud axis in negative direction */
#define ALT_NEG   0x2

/** Movement in the Azimuth axis in positive direction */
#define AZM_POS   0x4

/** Movement in the Azimuth axis in negative direction */
#define AZM_NEG   0x8

/** Max value for axis rotation in non-precise commands */
#define MAX_ROTATION   4294967296.0

/** Max value for axis rotation in precise commands */
#define MAX_PRECISE_ROTATION   4294967296.0

class Communication{

	private:
	SerialRS232 *sp;

	public:
	Communication(char *deviceName);
	~Communication();
	char *echo(char c);
	bool alignmentComplete();
	bool Slew(int rate, int direction);
	bool goToAltAzm(double alt, double azm);
	double getAlt();
	double getAzm();
};
