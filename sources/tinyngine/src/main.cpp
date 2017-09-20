#include "PlatformBridgeWin32.h"

int main(int _argc, char** _argv) {
	TINYNGINE_UNUSED(_argc); TINYNGINE_UNUSED(_argv);

	tinyngine::IPlatformBridge* platformInstance = new tinyngine::PlatformBridgeWin32();
	return platformInstance->Run();
}
