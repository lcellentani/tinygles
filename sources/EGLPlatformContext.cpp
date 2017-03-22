#include "EGLPlatformContext.h"
#include "EGLTrampoline.h"
#include "Log.h"
#include "StringUtils.h"

#include <sstream>

namespace tinygles
{

extern "C" IPlatformContext* CreatePlatformContext(void* windowHandle, const ContextAttribs& attributes) {
	if (!egl::InitTrampoline()) {
		return nullptr;
	}
	return new EGLPlatformContext({ reinterpret_cast<NativeWindow>(windowHandle) }, attributes);
}

//=====================================================================================================================

EGLPlatformContext::EGLPlatformContext(const NativeWindowHandle& handle, const ContextAttribs& attributes)
	: mWindowHandle(handle)
	, mEGLDisplay(0)
	, mEGLContext(0)
	, mEGLSurface(0)
	, mAttributes(attributes) {

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

std::string EGLPlatformContext::GetInfo() {
	std::ostringstream lines;
	std::string tmp;
	//EGLint i32Values[5];

	tmp.reserve(4096);

	lines << "\nEGL:\n";

	tmp = StringUtils::createFormatted("\tVendor:   %s\n", (const char*)egl::QueryString(mEGLDisplay, EGL_VENDOR));
	lines << tmp;

	tmp = StringUtils::createFormatted("\tVersion:  %s\n", (const char*)egl::QueryString(mEGLDisplay, EGL_VERSION));
	lines << tmp;

	tmp = StringUtils::createFormatted("\tExtensions:  %s\n", (const char*)egl::QueryString(mEGLDisplay, EGL_EXTENSIONS));
	lines << tmp;

	/*if (egl::QueryContext(m_platformContextHandles->display, m_platformContextHandles->context, EGL_CONTEXT_PRIORITY_LEVEL_IMG, &i32Values[0]))
	{
		switch (i32Values[0])
		{
		case EGL_CONTEXT_PRIORITY_HIGH_IMG:
			out.append("\tContext priority: High\n");
			break;
		case EGL_CONTEXT_PRIORITY_MEDIUM_IMG:
			out.append("\tContext priority: Medium\n");
			break;
		case EGL_CONTEXT_PRIORITY_LOW_IMG:
			out.append("\tContext priority: Low\n");
			break;
		default:
			out.append("\tContext priority: Unrecognised.\n");
		}
	}
	else
	{
		egl::GetError(); // Clear error
		out.append("\tContext priority: Unsupported\n");
	}

#if defined(EGL_VERSION_1_2)
	tmp = strings::createFormatted("\tClient APIs:  %hs\n",
		(const char*)egl::QueryString(m_platformContextHandles->display,
			EGL_CLIENT_APIS));
	out.append(tmp);
#endif

	egl::QuerySurface(m_platformContextHandles->display, m_platformContextHandles->drawSurface, EGL_WIDTH, &i32Values[0]);
	tmp = strings::createFormatted("\nSurface Width:  %i\n", i32Values[0]);
	out.append(tmp);

	egl::QuerySurface(m_platformContextHandles->display, m_platformContextHandles->drawSurface, EGL_HEIGHT, &i32Values[0]);
	tmp = strings::createFormatted("Surface Height: %i\n\n", i32Values[0]);
	out.append(tmp);

	// EGLSurface details

	// Get current config
	EGLConfig config;
	egl::QueryContext(m_platformContextHandles->display, m_platformContextHandles->context, EGL_CONFIG_ID, &i32Values[0]);
	const EGLint attributes[] = { EGL_CONFIG_ID, i32Values[0], EGL_NONE };
	egl::ChooseConfig(m_platformContextHandles->display, attributes, &config, 1, &i32Values[1]);

	out.append("EGL Surface:\n");

	tmp = strings::createFormatted("\tConfig ID:\t%i\n", i32Values[0]);
	out.append(tmp);

	// Color buffer
	egl::GetConfigAttrib(m_platformContextHandles->display, config, EGL_BUFFER_SIZE, &i32Values[0]);
	egl::GetConfigAttrib(m_platformContextHandles->display, config, EGL_RED_SIZE, &i32Values[1]);
	egl::GetConfigAttrib(m_platformContextHandles->display, config, EGL_GREEN_SIZE, &i32Values[2]);
	egl::GetConfigAttrib(m_platformContextHandles->display, config, EGL_BLUE_SIZE, &i32Values[3]);
	egl::GetConfigAttrib(m_platformContextHandles->display, config, EGL_ALPHA_SIZE, &i32Values[4]);
	tmp = strings::createFormatted("\tColor Buffer:  %i bits (R%i G%i B%i A%i)\n", i32Values[0], i32Values[1],
		i32Values[2], i32Values[3], i32Values[4]);
	out.append(tmp);

	// Depth buffer
	egl::GetConfigAttrib(m_platformContextHandles->display, config, EGL_DEPTH_SIZE, &i32Values[0]);
	tmp = strings::createFormatted("\tDepth Buffer:   %i bits\n", i32Values[0]);
	out.append(tmp);

	// Stencil Buffer
	egl::GetConfigAttrib(m_platformContextHandles->display, config, EGL_STENCIL_SIZE, &i32Values[0]);
	tmp = strings::createFormatted("\tStencil Buffer: %i bits\n", i32Values[0]);
	out.append(tmp);

	// EGL surface bits support
	egl::GetConfigAttrib(m_platformContextHandles->display, config, EGL_SURFACE_TYPE, &i32Values[0]);
	tmp = strings::createFormatted("\tSurface type:   %hs%hs%hs\n", i32Values[0] & EGL_WINDOW_BIT ? "WINDOW " : "",
		i32Values[1] & EGL_PBUFFER_BIT ? "PBUFFER " : "",
		i32Values[2] & EGL_PIXMAP_BIT ? "PIXMAP " : "");
	out.append(tmp);

	// EGL renderable type
#if defined(EGL_VERSION_1_2)
	egl::GetConfigAttrib(m_platformContextHandles->display, config, EGL_RENDERABLE_TYPE, &i32Values[0]);
	tmp = strings::createFormatted("\tRenderable type: %hs%hs%hs%hs\n",
		i32Values[0] & EGL_OPENVG_BIT ? "OPENVG " : "",
		i32Values[0] & EGL_OPENGL_ES_BIT ? "OPENGL_ES " : "",
#if defined(EGL_OPENGL_BIT)
		i32Values[0] & EGL_OPENGL_BIT ? "OPENGL " : "",
#endif
		i32Values[0] & EGL_OPENGL_ES2_BIT ? "OPENGL_ES2 " : "");
	out.append(tmp);
#endif

	egl::GetConfigAttrib(m_platformContextHandles->display, config, EGL_SAMPLE_BUFFERS, &i32Values[0]);
	egl::GetConfigAttrib(m_platformContextHandles->display, config, EGL_SAMPLES, &i32Values[1]);
	tmp = strings::createFormatted("\tSample buffer No.: %i\n", i32Values[0]);
	out.append(tmp);
	tmp = strings::createFormatted("\tSamples per pixel: %i", i32Values[1]);
	out.append(tmp);*/

	return lines.str();
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

	eglext::InitExtensions(egl::QueryString(mEGLDisplay, EGL_EXTENSIONS));

	Api targetApi = mAttributes.mRequiredApi;
	if (targetApi == Api::Unspecified) {

	}
	else {
		Log(Log.Information, "Forcing specific API level: %s", GetApiName(targetApi));
	}

	Log(Log.Information, "%s", GetInfo().c_str());

	return Result::Success;
}

Result EGLPlatformContext::InitializeContext() {
	int requestedMajorVersion = -1;
	int requestedMinorVersion = -1;

	switch (mAttributes.mRequiredApi)
	{
	case Api::OpenGLES2:
		requestedMajorVersion = 2;
		requestedMinorVersion = 0;
		break;
	case Api::OpenGLES3:
		requestedMajorVersion = 3;
		requestedMinorVersion = 0;
		break;
	case Api::OpenGLES31:
		requestedMajorVersion = 3;
		requestedMinorVersion = 1;
		break;
	default:
		break;
	}

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