#pragma once

#include "EventQueue.h"

namespace tinyngine
{

class IPlatformBridge {
public:
	virtual std::unique_ptr<Event> PollEvents() = 0;

protected:
	std::unique_ptr<EventQueue> mEventQueue;
};

} // namespace tinyngine