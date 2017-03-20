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
	EGLPlatformContext(const NativeWindowHandle& handle, Api requiredApi = Api::Unspecified);

	Result Initialize() override;
	Result Terminate() override;
	Result Present() override;
	Result MakeCurrent() override;

private:
	Result InitializeBinding();
	Result InitializeContext();

private:
	NativeWindowHandle mWindowHandle;
	EGLDisplay mEGLDisplay;
	EGLContext mEGLContext;
	EGLSurface mEGLSurface;

	Api mRequiredApi = Api::Unspecified;
};

} // namespace tinygles