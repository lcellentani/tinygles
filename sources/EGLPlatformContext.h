#pragma once

#include "IPlatformContext.h"
#include "EGLApi.h"

namespace tinygles
{

typedef EGLNativeWindowType NativeWindow;

struct NativeWindowHandle {
	NativeWindow mNativeWindow;

	operator NativeWindow& () { return mNativeWindow; }
	operator const NativeWindow& () const { return mNativeWindow; }
};

class EGLPlatformContext : public IPlatformContext {
public:
	EGLPlatformContext(const NativeWindowHandle& handle, const ContextAttribs& attributes);

	Result Initialize() override;
	Result Terminate() override;
	Result Present() override;
	Result MakeCurrent() override;

	std::string GetInfo() override;

private:
	Result InitializeBinding();
	Result InitializeContext();

private:
	NativeWindowHandle mWindowHandle;
	EGLDisplay mEGLDisplay;
	EGLContext mEGLContext;
	EGLSurface mEGLSurface;

	ContextAttribs mAttributes;
};

} // namespace tinygles