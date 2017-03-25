#include "EGLPlatformContext.h"
#include "EGLTrampoline.h"
#include "Log.h"
#include "StringUtils.h"

#include <sstream>
#include <algorithm>
#include <vector>

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
	Result result = Result::Success;
	result = InitializeBinding();
	if (result != Result::Success) {
		return result;
	}

	EGLConfig config;
	result = InitializeContext(config, true);
	if (result != Result::Success) {
		return result;
	}

	EGLint eglattribs[] = { EGL_NONE, EGL_NONE, EGL_NONE };

	//if (m_OSManager.getDisplayAttributes().frameBufferSrgb)
	//{
	//	bool isSrgbSupported = egl::isEglExtensionSupported(m_platformContextHandles->display, "EGL_KHR_gl_colorspace");
	//	if (isSrgbSupported)
	//	{
	//		eglattribs[0] = EGL_COLORSPACE;
	//		eglattribs[1] = EGL_COLORSPACE_sRGB;
	//	}
	//	else
	//	{
	//		Log(Log.Warning, "sRGB window backbuffer requested, but EGL_KHR_gl_colorspace is not supported. Creating linear RGB backbuffer.");
	//		m_OSManager.getDisplayAttributes().frameBufferSrgb = false;
	//	}
	//}

	mEGLSurface = egl::CreateWindowSurface(mEGLDisplay, config, mWindowHandle.mNativeWindow, eglattribs);
	if (mEGLSurface == EGL_NO_SURFACE) {
		Log(Log.Error, "Context creation failed\n");
		return Result::InvalidArgument;
	}

	egl::QuerySurface(mEGLDisplay, mEGLSurface, EGL_WIDTH, (EGLint*)&mSurfaceWidth);
	egl::QuerySurface(mEGLDisplay, mEGLSurface, EGL_HEIGHT, (EGLint*)&mSurfaceHeight);

	//m_swapInterval = 1;

	if (!egl::MakeCurrent(mEGLDisplay, mEGLSurface, mEGLSurface, mEGLContext)) {
		return Result::InvalidArgument;
	}

	Log(Log.Information, "%s", GetInfo().c_str());

	return Result::Success;
}

Result EGLPlatformContext::Terminate() {
	egl::MakeCurrent(mEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
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
	EGLint i32Values[5];

	tmp.reserve(4096);

	lines << "\nEGL:\n";

	tmp = StringUtils::createFormatted("\tVendor:   %s\n", (const char*)egl::QueryString(mEGLDisplay, EGL_VENDOR));
	lines << tmp;
	tmp = StringUtils::createFormatted("\tVersion:  %s\n", (const char*)egl::QueryString(mEGLDisplay, EGL_VERSION));
	lines << tmp;
	tmp = StringUtils::createFormatted("\tExtensions:  %s\n", (const char*)egl::QueryString(mEGLDisplay, EGL_EXTENSIONS));
	lines << tmp;

	if (egl::QueryContext(mEGLDisplay, mEGLContext, EGL_CONTEXT_PRIORITY_LEVEL_IMG, &i32Values[0])) {
		switch (i32Values[0]) {
		case EGL_CONTEXT_PRIORITY_HIGH_IMG:
			lines << "\tContext priority: High\n";
			break;
		case EGL_CONTEXT_PRIORITY_MEDIUM_IMG:
			lines << "\tContext priority: Medium\n";
			break;
		case EGL_CONTEXT_PRIORITY_LOW_IMG:
			lines << "\tContext priority: Low\n";
			break;
		default:
			lines << "\tContext priority: Unrecognised.\n";
		}
	}
	else {
		egl::GetError();
		lines << "\tContext priority: Unsupported\n";
	}

#if defined(EGL_VERSION_1_2)
	tmp = StringUtils::createFormatted("\tClient APIs:  %s\n", (const char*)egl::QueryString(mEGLDisplay, EGL_CLIENT_APIS));
	lines << tmp;
#endif

	egl::QuerySurface(mEGLDisplay, mEGLSurface, EGL_WIDTH, &i32Values[0]);
	tmp = StringUtils::createFormatted("\nSurface Width:  %i\n", i32Values[0]);
	lines << tmp;
	egl::QuerySurface(mEGLDisplay, mEGLSurface, EGL_HEIGHT, &i32Values[0]);
	tmp = StringUtils::createFormatted("Surface Height: %i\n\n", i32Values[0]);
	lines << tmp;

	EGLConfig config;
	egl::QueryContext(mEGLDisplay, mEGLContext, EGL_CONFIG_ID, &i32Values[0]);
	const EGLint attributes[] = { EGL_CONFIG_ID, i32Values[0], EGL_NONE };
	egl::ChooseConfig(mEGLDisplay, attributes, &config, 1, &i32Values[1]);

	lines << ("EGL Surface:\n");

	tmp = StringUtils::createFormatted("\tConfig ID:\t%i\n", i32Values[0]);
	lines << tmp;

	// Color buffer
	egl::GetConfigAttrib(mEGLDisplay, config, EGL_BUFFER_SIZE, &i32Values[0]);
	egl::GetConfigAttrib(mEGLDisplay, config, EGL_RED_SIZE, &i32Values[1]);
	egl::GetConfigAttrib(mEGLDisplay, config, EGL_GREEN_SIZE, &i32Values[2]);
	egl::GetConfigAttrib(mEGLDisplay, config, EGL_BLUE_SIZE, &i32Values[3]);
	egl::GetConfigAttrib(mEGLDisplay, config, EGL_ALPHA_SIZE, &i32Values[4]);
	tmp = StringUtils::createFormatted("\tColor Buffer:  %i bits (R%i G%i B%i A%i)\n", i32Values[0], i32Values[1], i32Values[2], i32Values[3], i32Values[4]);
	lines << tmp;
	// Depth buffer
	egl::GetConfigAttrib(mEGLDisplay, config, EGL_DEPTH_SIZE, &i32Values[0]);
	tmp = StringUtils::createFormatted("\tDepth Buffer:   %i bits\n", i32Values[0]);
	lines << tmp;
	// Stencil Buffer
	egl::GetConfigAttrib(mEGLDisplay, config, EGL_STENCIL_SIZE, &i32Values[0]);
	tmp = StringUtils::createFormatted("\tStencil Buffer: %i bits\n", i32Values[0]);
	lines << tmp;
	// EGL surface bits support
	egl::GetConfigAttrib(mEGLDisplay, config, EGL_SURFACE_TYPE, &i32Values[0]);
	tmp = StringUtils::createFormatted("\tSurface type:   %s%s%s\n", i32Values[0] & EGL_WINDOW_BIT ? "WINDOW " : "", i32Values[1] & EGL_PBUFFER_BIT ? "PBUFFER " : "", i32Values[2] & EGL_PIXMAP_BIT ? "PIXMAP " : "");
	lines << tmp;

	// EGL renderable type
#if defined(EGL_VERSION_1_2)
	egl::GetConfigAttrib(mEGLDisplay, config, EGL_RENDERABLE_TYPE, &i32Values[0]);
	tmp = StringUtils::createFormatted("\tRenderable type: %s%s%s%s\n",
		i32Values[0] & EGL_OPENVG_BIT ? "OPENVG " : "",
		i32Values[0] & EGL_OPENGL_ES_BIT ? "OPENGL_ES " : "",
#if defined(EGL_OPENGL_BIT)
		i32Values[0] & EGL_OPENGL_BIT ? "OPENGL " : "",
#endif
		i32Values[0] & EGL_OPENGL_ES2_BIT ? "OPENGL_ES2 " : "");
	lines << tmp;
#endif

	egl::GetConfigAttrib(mEGLDisplay, config, EGL_SAMPLE_BUFFERS, &i32Values[0]);
	egl::GetConfigAttrib(mEGLDisplay, config, EGL_SAMPLES, &i32Values[1]);
	tmp = StringUtils::createFormatted("\tSample buffer No.: %i\n", i32Values[0]);
	lines << tmp;
	tmp = StringUtils::createFormatted("\tSamples per pixel: %i", i32Values[1]);
	lines << tmp;

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

	return Result::Success;
}

Result EGLPlatformContext::InitializeContext(EGLConfig& config, bool wantsWindow) {
	int requestedMajorVersion = -1;
	int requestedMinorVersion = -1;

	switch (mAttributes.mRequiredApi) {
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

	bool createContextSupported = egl::isEglExtensionSupported(mEGLDisplay, "EGL_KHR_create_context");
	if (createContextSupported) {
		Log(Log.Information, "EGL context creation: EGL_KHR_create_context supported...");
	}
	else {
		Log(requestedMinorVersion ? Log.Warning : Log.Information, "EGL context creation: EGL_KHR_create_context not supported. Minor version will be discarded, and debug disabled.");
		requestedMinorVersion = 0;
	}

	bool contextPrioritySupported = egl::isEglExtensionSupported(mEGLDisplay, "EGL_IMG_context_priority");
	if (contextPrioritySupported) {
		switch (mAttributes.mContextPriority) {
		case 0: Log(Log.Information, "EGL context creation: EGL_IMG_context_priority supported! Setting context LOW priority..."); break;
		case 1: Log(Log.Information, "EGL context creation: EGL_IMG_context_priority supported! Setting context MEDIUM priority..."); break;
		default: Log(Log.Information, "EGL context creation: EGL_IMG_context_priority supported! Setting context HIGH priority (default)..."); break;
		}
	}
	else {
		Log(Log.Information, "EGL context creation: EGL_IMG_context_priority not supported. Ignoring context Priority attribute.");
	}

	EGLint configAttributes[32]{ 0 };
	bool debugBit = false;
#ifdef _DEBUG
	debugBit = true;
#endif
	uint32_t i = 0;
	Log(Log.Debug, "Attempting to create context with:");
	Log(Log.Debug, "\tDebugbit: %s", debugBit ? "true" : "false");
	Log(Log.Debug, "\tRedBits: %d", mAttributes.mRedBits);
	Log(Log.Debug, "\tGreenBits: %d", mAttributes.mGreenBits);
	Log(Log.Debug, "\tBlueBits: %d", mAttributes.mBlueBits);
	Log(Log.Debug, "\tAlphaBits: %d", mAttributes.mAlphaBits);
	Log(Log.Debug, "\tDepthBits: %d", mAttributes.mDepthBPP);
	Log(Log.Debug, "\tStencilBits: %d", mAttributes.mStencilBPP);

	//if (mAttributes.configID > 0)
	//{
	//	configAttributes[i++] = EGL_CONFIG_ID;
	//	configAttributes[i++] = attributes.configID;
	//}
	//else
	{
		configAttributes[i++] = EGL_RED_SIZE;
		configAttributes[i++] = mAttributes.mRedBits;

		configAttributes[i++] = EGL_GREEN_SIZE;
		configAttributes[i++] = mAttributes.mGreenBits;

		configAttributes[i++] = EGL_BLUE_SIZE;
		configAttributes[i++] = mAttributes.mBlueBits;

		configAttributes[i++] = EGL_ALPHA_SIZE;
		configAttributes[i++] = mAttributes.mAlphaBits;

		mAttributes.mDepthBPP = (std::min)(mAttributes.mDepthBPP, 24u);
		configAttributes[i++] = EGL_DEPTH_SIZE;
		configAttributes[i++] = mAttributes.mDepthBPP;

		configAttributes[i++] = EGL_STENCIL_SIZE;
		configAttributes[i++] = mAttributes.mStencilBPP;

		if (wantsWindow) {
			configAttributes[i++] = EGL_SURFACE_TYPE;
			configAttributes[i++] = EGL_WINDOW_BIT;
		}

		switch (mAttributes.mRequiredApi) {
		case Api::OpenGLES2:
			Log(Log.Debug, "EGL context creation: Setting EGL_OPENGL_ES2_BIT");
			configAttributes[i++] = EGL_RENDERABLE_TYPE;
			configAttributes[i++] = EGL_OPENGL_ES2_BIT;
			break;
		case Api::OpenGLES3:
		case Api::OpenGLES31:
			Log(Log.Debug, "EGL context creation: EGL_OPENGL_ES3_BIT");
			configAttributes[i++] = EGL_RENDERABLE_TYPE;
			configAttributes[i++] = EGL_OPENGL_ES3_BIT_KHR;
			break;
		default:
			return Result::UnsupportedRequest;
			break;
		}

		if (mAttributes.mSamplesAA > 0) {
			Log(Log.Debug, "EGL context creation: EGL_SAMPLE_BUFFERS 1");
			Log(Log.Debug, "EGL context creation: EGL_SAMPLES %d", mAttributes.mSamplesAA);
			configAttributes[i++] = EGL_SAMPLE_BUFFERS;
			configAttributes[i++] = 1;
			configAttributes[i++] = EGL_SAMPLES;
			configAttributes[i++] = mAttributes.mSamplesAA;
		}
	}

	configAttributes[i] = EGL_NONE;

	EGLint numConfigs;
	EGLint configsSize;
	std::vector<EGLConfig> configs;

	EGLint eglerror = egl::GetError();
	//assertion(eglerror == EGL_SUCCESS, "initializeContext: egl error logged before choosing egl config");
	eglerror = egl::ChooseConfig(mEGLDisplay, configAttributes, NULL, 0, &configsSize);
	//assertion(eglerror == EGL_TRUE, "initializeContext: EGL config returned a value that was not EGL_TRUE");
	eglerror = egl::GetError();
	//assertion(eglerror == EGL_SUCCESS, "initializeContext: EGL choose config raised EGL error");

	//if (attributes.forceColorBPP)
	//{
	//	if (configsSize == 0) { return Result::UnknownError; }
	//}
	//else
	{
		if (configsSize > 1) { configsSize = 1; }
	}
	numConfigs = configsSize;
	if (configsSize) {
		configs.resize(configsSize);
		if (egl::ChooseConfig(mEGLDisplay, configAttributes, configs.data(), configsSize, &numConfigs) != EGL_TRUE) {
			Log("EGL context creation: initializeContext Error choosing egl config. %x.    Expected number of configs: %d    Actual: %d.", egl::GetError(), numConfigs, configsSize);
			return Result::UnsupportedRequest;
		}
	}
	Log(Log.Information, "EGL context creation: Number of EGL Configs found: %d", configsSize);

	if (numConfigs > 0) {
		EGLint configIdx = 0;
		//if (attributes.forceColorBPP)
		//{
		//	Log(Log.Information, "EGL context creation: Trying to find a for forced BPP compatible context support...");
		//	EGLint value;
		//
		//	for (; configIdx < configsSize; ++configIdx)
		//	{
		//		if ((egl::GetConfigAttrib(handles->display, configs[configIdx], EGL_RED_SIZE, &value)
		//			&& value == static_cast<EGLint>(original_attributes.redBits))
		//			&& (egl::GetConfigAttrib(handles->display, configs[configIdx], EGL_GREEN_SIZE, &value)
		//				&& value == static_cast<EGLint>(original_attributes.greenBits))
		//			&& (egl::GetConfigAttrib(handles->display, configs[configIdx], EGL_BLUE_SIZE, &value)
		//				&& value == static_cast<EGLint>(original_attributes.blueBits))
		//			&& (egl::GetConfigAttrib(handles->display, configs[configIdx], EGL_ALPHA_SIZE, &value)
		//				&& value == static_cast<EGLint>(original_attributes.alphaBits))
		//			)
		//		{
		//			break;
		//		}
		//	}
		//}

		config = configs[configIdx];

		EGLint contextAttributes[32];
		i = 0;


#if defined(EGL_CONTEXT_MAJOR_VERSION_KHR)
		if (createContextSupported) {
			contextAttributes[i++] = EGL_CONTEXT_MAJOR_VERSION_KHR;
			contextAttributes[i++] = requestedMajorVersion;
			contextAttributes[i++] = EGL_CONTEXT_MINOR_VERSION_KHR;
			contextAttributes[i++] = requestedMinorVersion;
#ifdef _DEBUG
			if (debugBit) {
				contextAttributes[i++] = EGL_CONTEXT_FLAGS_KHR;
				contextAttributes[i++] = EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR;
			}
#endif
		}
		else
#endif
		{
			contextAttributes[i++] = EGL_CONTEXT_CLIENT_VERSION;
			contextAttributes[i++] = requestedMajorVersion;
		}

		if (contextPrioritySupported) {
			contextAttributes[i++] = EGL_CONTEXT_PRIORITY_LEVEL_IMG;
			switch (mAttributes.mContextPriority) {
			case 0: contextAttributes[i++] = EGL_CONTEXT_PRIORITY_LOW_IMG; break;
			case 1: contextAttributes[i++] = EGL_CONTEXT_PRIORITY_MEDIUM_IMG; break;
			default: contextAttributes[i++] = EGL_CONTEXT_PRIORITY_HIGH_IMG; break;
			}
		}
		contextAttributes[i] = EGL_NONE;

		Log(Log.Information, "Creating EGL context...");
		mEGLContext = egl::CreateContext(mEGLDisplay, config, NULL, contextAttributes);
		if (mEGLContext != EGL_NO_CONTEXT) {
			Log(Log.Debug, "EGL context successfully created! Updating Config Attributes to reflect actual context parameters...");

			egl::GetConfigAttrib(mEGLDisplay, config, EGL_RED_SIZE, (EGLint*)&mAttributes.mRedBits);
			egl::GetConfigAttrib(mEGLDisplay, config, EGL_GREEN_SIZE, (EGLint*)&mAttributes.mGreenBits);
			egl::GetConfigAttrib(mEGLDisplay, config, EGL_BLUE_SIZE, (EGLint*)&mAttributes.mBlueBits);
			egl::GetConfigAttrib(mEGLDisplay, config, EGL_ALPHA_SIZE, (EGLint*)&mAttributes.mAlphaBits);
			egl::GetConfigAttrib(mEGLDisplay, config, EGL_DEPTH_SIZE, (EGLint*)&mAttributes.mDepthBPP);
			egl::GetConfigAttrib(mEGLDisplay, config, EGL_STENCIL_SIZE, (EGLint*)&mAttributes.mStencilBPP);
			Log(Log.Information, "EGL Initialized Successfully");
			return Result::Success;
		}
	}
	eglerror = egl::GetError();
	if (eglerror != EGL_SUCCESS) {
		Log(Log.Debug, "Context not created yet. Clearing EGL errors.");
	}
	return Result::NotInitialized;
}

} // namespace tinygles