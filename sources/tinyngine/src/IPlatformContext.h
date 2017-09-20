#pragma once

#include "PlatformTypes.h"
#include <string>

namespace tinyngine
{
class IPlatformContext {
public:
	virtual Result Initialize() = 0;

	virtual Result Terminate() = 0;

	virtual Result Present() = 0;

	virtual Result MakeCurrent() = 0;

	virtual std::string GetInfo() = 0;
};

extern "C" IPlatformContext* CreatePlatformContext(void* windowHandle, const ContextAttribs& attributes);

} // namespace tinyngine