#pragma once

#include "PlatformDefine.h"

namespace tinyngine
{

class IPlatformBridge {
public:
	virtual ~IPlatformBridge() = default;

	virtual int16_t Run() = 0;
};

} // namespace tinyngine