#include "PlatformBridge.h"
#include "math/Array.h"

int main(int _argc, char** _argv) {
	UNUSED(_argc); UNUSED(_argv);

	tinygles::PlatformBridge platformInstance;
	return platformInstance.Run();
	return 0;
}
