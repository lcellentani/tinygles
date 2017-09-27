#pragma once

#include "PlatformDefine.h"
#include "StopWatch.h"

namespace tinyngine
{

class IPlatformBridge {
public:
	virtual ~IPlatformBridge() = default;

	virtual int16_t Run() = 0;

protected:
	float mFrameDelta;
	std::unique_ptr<StopWatch> mStopWatch;
};

} // namespace tinyngine