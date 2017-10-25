#pragma once

#include "PlatformDefine.h"
#include "PlatformTypes.h"
#include "SpScLockFreeQueue.h"

#include <memory>

namespace tinyngine
{

class Event {
public:
	enum Type {
		Null,
		Exit,
		Key,
		Mouse,
		Size,
		Char
	};

	Event() : mType(Type::Null) {}
	Event(Type type) : mType(type) {}
	virtual ~Event() = default;

	Type mType;
};

class ExitEvent : public Event {
public:
	ExitEvent() : Event(Event::Exit) {}
};

class SizeEvent : public Event {
public:
	SizeEvent() = delete;
	explicit SizeEvent(uint32_t width, uint32_t height) : Event(Event::Size), mWidth(width), mHeight(height) {}

	uint32_t mWidth;
	uint32_t mHeight;
};

class KeyEvent : public Event {
public:
	KeyEvent() = delete;
	explicit KeyEvent(Key::Enum key, uint8_t modifiers, bool pressed) : Event(Event::Key), mKey(key), mModifilers(modifiers), mPressed(pressed) {}

	Key::Enum mKey;
	uint8_t mModifilers;
	bool mPressed;
};

class CharEvent : public Event {
public:
	CharEvent() = delete;
	CharEvent(uint32_t codepoint, std::array<uint8_t, 4>& ch, uint8_t len) : Event(Event::Char), mCodepoint(codepoint), mChar(ch), mLength(len) {}

	uint32_t mCodepoint;
	std::array<uint8_t, 4> mChar;
	uint8_t mLength;
};

class MouseEvent : public Event {
public:
	MouseEvent() = delete;
	MouseEvent(int32_t x, int32_t y, MouseButton::Enum button, bool pressed) : Event(Event::Mouse), mPosX(x), mPosY(y), mButton(button), mPressed(pressed) {}

	int32_t mPosX;
	int32_t mPosY;
	MouseButton::Enum mButton;
	uint8_t mPressed;
};

class EventQueue {
public:
	void postExitEvent() {
		mQueue.push(std::make_unique<ExitEvent>());
	}

	void postSizeEvent(uint32_t width, uint32_t height) {
		mQueue.push(std::make_unique<SizeEvent>(width, height));
	}

	void postKeyEvent(Key::Enum key, uint8_t modifier, bool pressed) {
		mQueue.push(std::make_unique<KeyEvent>(key, modifier, pressed));
	}

	void postCharEvent(uint32_t codepoint, std::array<uint8_t, 4>& ch, uint8_t len) {
		mQueue.push(std::make_unique<CharEvent>(codepoint, ch, len));
	}

	void postMouseEvent(int32_t x, int32_t y, MouseButton::Enum button, bool pressed) {
		mQueue.push(std::make_unique<MouseEvent>(x, y, button, pressed));
	}

	std::unique_ptr<Event> poll() {
		std::unique_ptr<Event> event;
		mQueue.pop(event);
		return event;
	}

private:
	static constexpr size_t QueueSize = 64;
	SpScLockFreeQueue<std::unique_ptr<Event>, QueueSize> mQueue;
};

} // namespace tinyngine