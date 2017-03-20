#pragma once

#include "PlatformTypes.h"

namespace tinygles
{
class IPlatformContext {
public:
	virtual Result Initialize() = 0;

	virtual Result Terminate() = 0;

	virtual Result Present() = 0;

	virtual Result MakeCurrent() = 0;
};

extern "C" IPlatformContext* CreatePlatformContext(void* windowHandle, Api requiredApi = Api::Unspecified);

} // namespace tinygles