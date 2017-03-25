#pragma once

#include <cstdint>

namespace tinygles
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

} // namespace tinygles