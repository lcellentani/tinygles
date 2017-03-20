#include "PlatformBridge.h"

int main(int _argc, char** _argv) {
	tinygles::PlatformBridge platformInstance;
	return platformInstance.Run();
}
