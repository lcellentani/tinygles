#pragma once

#include "PlatformDefine.h"

namespace tinyngine
{

enum class Api
{
	Unspecified = 0,
	OpenGLES2,
	OpenGLES3,
	OpenGLES31,
	OpenGLESMaxVersion = OpenGLES31,
	Vulkan,
	Count
};

inline const char* GetApiCode(Api api)
{
	static const char* ApiCodes[] =
	{
		"",
		"ES2",
		"ES3",
		"ES31",
		"vk",
	};
	return ApiCodes[(int)api];
}

inline const char* GetApiName(Api api)
{
	static const char* ApiCodes[] =
	{
		"Unknown",
		"OpenGL ES 2.0",
		"OpenGL ES 3.0",
		"OpenGL ES 3.1",
		"Vulkan",
	};
	return ApiCodes[(int)api];
}

enum class VsyncMode
{
	Off,		// The application does not synchronizes with the vertical sync. If application renders faster than the display refreshes, frames are wasted and tearing may be observed. FPS is uncapped. Maximum power consumption. If unsupported, "ON" value will be used instead. Minimum latency.
	On,			// The application is always syncrhonized with the vertical sync. Tearing does not happen. FPS is capped to the display's refresh rate. For fast applications, battery life is improved. Always supported.
	Relaxed,	// The application synchronizes with the vertical sync, but only if the application rendering speed is greater than refresh rate. Compared to OFF, there is no tearing. Compared to ON, the FPS will be improved for "slower" applications. If unsupported, "ON" value will be used instead. Recommended for most applications. Default if supported.
	Mailbox,	// The presentation engine will always use the latest fully rendered image. Compared to OFF, no tearing will be observed. Compared to ON, battery power will be worse, especially for faster applications. If unsupported,  "OFF" will be attempted next.
	Half,		// The application is capped to using half the vertical sync time. FPS artificially capped to Half the display speed (usually 30fps) to maintain battery. Best possible battery savings. Worst possibly performance. Recommended for specific applications where battery saving is critical.
};


enum class Result
{
	Success = 0,
	UnknownError,

	OutOfMemory,
	InvalidArgument,
	AlreadyInitialized,
	NotInitialized,
	UnsupportedRequest,
	FileVersionMismatch,

	NotReadable,
	NotWritable,
	EndOfStream,
	UnableToOpen,
	NoData,

	OutOfBounds,
	NotFound,

	KeyAlreadyExists,

	InvalidData,
};

struct NativeWindowHandle;

struct ContextAttribs {
	Api mRequiredApi;
	uint32_t mDepthBPP;
	uint32_t mStencilBPP;
	uint32_t mRedBits;
	uint32_t mGreenBits;
	uint32_t mBlueBits;
	uint32_t mAlphaBits;
	uint32_t mSamplesAA;
	VsyncMode mVsyncMode;
	uint32_t mContextPriority;
};

struct Modifier {
	enum Enum {
		None = 0,
		LeftAlt = 1 << 0,
		RightAlt = 1 << 1,
		LeftCtrl = 1 << 2,
		RightCtrl = 1 << 3,
		LeftShift = 1 << 4,
		RightShift = 1 << 5,
		LeftMeta = 1 << 6,
		RightMeta = 1 << 7
	};
};

struct Key {
	enum Enum {
		None = 0,
		Esc,
		Return,
		Tab,
		Space,
		Backspace,
		Up,
		Down,
		Left,
		Right,
		Insert,
		Delete,
		Home,
		End,
		PageUp,
		PageDown,
		Print,
		Plus,
		Minus,
		LeftBracket,
		RightBracket,
		Semicolon,
		Quote,
		Comma,
		Period,
		Slash,
		Backslash,
		Tilde,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		NumPad0,
		NumPad1,
		NumPad2,
		NumPad3,
		NumPad4,
		NumPad5,
		NumPad6,
		NumPad7,
		NumPad8,
		NumPad9,
		Key0,
		Key1,
		Key2,
		Key3,
		Key4,
		Key5,
		Key6,
		Key7,
		Key8,
		Key9,
		KeyA,
		KeyB,
		KeyC,
		KeyD,
		KeyE,
		KeyF,
		KeyG,
		KeyH,
		KeyI,
		KeyJ,
		KeyK,
		KeyL,
		KeyM,
		KeyN,
		KeyO,
		KeyP,
		KeyQ,
		KeyR,
		KeyS,
		KeyT,
		KeyU,
		KeyV,
		KeyW,
		KeyX,
		KeyY,
		KeyZ,

		GamepadA,
		GamepadB,
		GamepadX,
		GamepadY,
		GamepadThumbL,
		GamepadThumbR,
		GamepadShoulderL,
		GamepadShoulderR,
		GamepadUp,
		GamepadDown,
		GamepadLeft,
		GamepadRight,
		GamepadBack,
		GamepadStart,
		GamepadGuide
	};
};

struct MouseButton {
	enum Enum {
		None = -1,
		Left,
		Middle,
		Right,
		Count
	};
};

struct MouseState {
	uint32_t mPosX = 0;
	uint32_t mPosY = 0;
	uint8_t mButtons[MouseButton::Enum::Count]{ 0 };
};

} // namespace tinyngine