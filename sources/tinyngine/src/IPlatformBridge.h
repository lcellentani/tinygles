#pragma once

#include "StopWatch.h"
#include "EventQueue.h"

namespace tinyngine
{

class IPlatformBridge {
public:
	virtual std::unique_ptr<Event> PollEvents() = 0;

//protected:
	float mFrameDelta;
	std::unique_ptr<StopWatch> mStopWatch;
	std::unique_ptr<EventQueue> mEventQueue;
};

} // namespace tinyngine