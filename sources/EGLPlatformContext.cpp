#include "EGLPlatformContext.h"
#include "EGLTrampoline.h"

namespace tinygles
{

extern "C" IPlatformContext* CreatePlatformContext(void* windowHandle, Api requiredApi /*= Api::Unspecified*/) {
	if (!egl::InitTrampoline()) {
		return nullptr;
	}
	return new EGLPlatformContext({ reinterpret_cast<NativeWindow>(windowHandle) });
}

//=====================================================================================================================

EGLPlatformContext::EGLPlatformContext(const NativeWindowHandle& handle, Api requiredApi /*= Api::Unspecified*/)
	: mWindowHandle(handle)
	, mEGLDisplay(0)
	, mEGLContext(0)
	, mEGLSurface(0)
	, mRequiredApi(requiredApi) {

}

Result EGLPlatformContext::Initialize() {
	InitializeBinding();

	InitializeContext();

	return Result::Success;
}

Result EGLPlatformContext::Terminate() {
	egl::MakeCurrent(mEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	// Terminate the display, and any resources associated with it (including the EGLContext)
	egl::Terminate(mEGLDisplay);
	return Result::Success;
}

Result EGLPlatformContext::Present() {
	egl::SwapBuffers(mEGLDisplay, mEGLSurface);
	return Result::Success;
}

Result EGLPlatformContext::MakeCurrent() {
	egl::MakeCurrent(mEGLDisplay, mEGLSurface, mEGLSurface, mEGLContext);
	return Result::Success;
}

Result EGLPlatformContext::InitializeBinding() {
	mEGLDisplay = egl::GetDisplay(GetDC(mWindowHandle.mNativeWindow));
	if (mEGLDisplay == EGL_NO_DISPLAY) {
		return Result::InvalidArgument;
	}

	EGLint majorVersion;
	EGLint minorVersion;
	if (!egl::Initialize(mEGLDisplay, &majorVersion, &minorVersion)) {
		return Result::InvalidArgument;
	}

	egl::BindAPI(EGL_OPENGL_ES_API);

	Api targetApi = mRequiredApi;
	if (mRequiredApi == Api::Unspecified) {

	}

	return Result::Success;
}

Result EGLPlatformContext::InitializeContext() {
	const EGLint configurationAttributes[] = {
		EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
		EGL_NONE
	};
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

	EGLConfig config;
	EGLint numConfigs;
	if (!egl::ChooseConfig(mEGLDisplay, configurationAttributes, &config, 1, &numConfigs)) {
		return Result::InvalidArgument;
	}

	mEGLSurface = egl::CreateWindowSurface(mEGLDisplay, config, mWindowHandle.mNativeWindow, nullptr);
	if (mEGLSurface == EGL_NO_SURFACE) {
		return Result::InvalidArgument;
	}

	mEGLContext = egl::CreateContext(mEGLDisplay, config, EGL_NO_CONTEXT, contextAttribs);
	if (mEGLContext == EGL_NO_CONTEXT) {
		return Result::InvalidArgument;
	}

	// Make the context current
	if (!egl::MakeCurrent(mEGLDisplay, mEGLSurface, mEGLSurface, mEGLContext)) {
		return Result::InvalidArgument;
	}

	return Result::Success;
}

} // namespace tinygles