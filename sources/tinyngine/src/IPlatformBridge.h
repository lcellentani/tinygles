#pragma once

#include "PlatformDefine.h"

namespace tinygles
{

class IPlatformBridge {
public:
	virtual ~IPlatformBridge() = default;

	virtual int16_t Run() = 0;
};

} // namespace tinygles