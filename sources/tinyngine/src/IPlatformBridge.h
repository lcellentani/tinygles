#pragma once

#include "StopWatch.h"
#include "EventQueue.h"

namespace tinyngine
{

class IPlatformBridge {
public:
	virtual bool PollEvents(Event* event) = 0;

//protected:
	float mFrameDelta;
	std::unique_ptr<StopWatch> mStopWatch;
	std::unique_ptr<EventQueue> mEventQueue;
};

} // namespace tinyngine