#pragma once

#include "SpScLockFreeQueue.h"

namespace tinyngine
{

class Event {
public:
	enum Type {
		Null,
		Exit,
		Key,
		Mouse,
		Size
	};

	Event() : mType(Type::Null) {}
	Event(Type type) : mType(type) {}

	Type mType;
};

class ExitEvent : public Event {
public:
	ExitEvent() : Event(Event::Exit) {}
};

class EventQueue {
public:
	EventQueue() = default;
	~EventQueue() = default;

	void postExitEvent() {
		mQueue.push(ExitEvent());
	}

	bool poll(Event* event) {
		return mQueue.tryPop(event);
	}

private:
	static constexpr size_t QueueSize = 64;
	SpScLockFreeQueue<Event, QueueSize> mQueue;
};

} // namespace tinyngine