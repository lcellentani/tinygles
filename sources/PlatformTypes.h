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
};

} // namespace tinygles