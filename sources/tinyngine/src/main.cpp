#include "PlatformBridgeWin32.h"

int main(int _argc, char** _argv) {
	TINYGLES_UNUSED(_argc); TINYGLES_UNUSED(_argv);

	tinygles::IPlatformBridge* platformInstance = new tinygles::PlatformBridgeWin32();
	return platformInstance->Run();
}
