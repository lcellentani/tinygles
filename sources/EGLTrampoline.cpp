#include "EGLTrampoline.h"
#include "DynLibLoader.h"

namespace {
	static tinygles::DynLibLoader& GetEGLLibrary() {
#ifdef _WIN32
		static constexpr const char* sEGLLibraryPath = "libEGL.dll";
#elif defined(TARGET_OS_MAC)
		static constexpr char* sEGLLibraryPath = "libEGL.dylib";
#else
		static constexpr char* sEGLLibraryPath = "libEGL.so";
#endif

		static tinygles::DynLibLoader sEGLLibrary(sEGLLibraryPath);
		return sEGLLibrary;
	}

	static bool isExtensionSupported(const char* const extensionString, const char* const extension) {
		if (!extensionString) {
			return false;
		}

		// The recommended technique for querying OpenGL extensions;
		// from http://opengl.org/resources/features/OGLextensions/
		const char* start = extensionString;
		char* position, *terminator;

		// Extension names should not have spaces.
		position = (char*)strchr(extension, ' ');

		if (position || *extension == '\0') {
			return 0;
		}

		/* It takes a bit of care to be fool-proof about parsing the
		OpenGL extensions string. Don't be fooled by sub-strings, etc. */
		for (;;) {
			position = (char*)strstr((char*)start, extension);
			if (!position) {
				break;
			}

			terminator = position + strlen(extension);

			if (position == start || *(position - 1) == ' ') {
				if (*terminator == ' ' || *terminator == '\0') {
					return true;
				}
			}
			start = terminator;
		}

		return false;
	}
}

bool egl::InitTrampoline() {
	tinygles::DynLibLoader libHandle = GetEGLLibrary();
	if (!libHandle.HasBeenLoaded()) {
		return false;
	}

	egl::ChooseConfig = libHandle.GetFunction<PROC_EGL_eglChooseConfig>("eglChooseConfig");
	egl::CopyBuffers = libHandle.GetFunction<PROC_EGL_eglCopyBuffers>("eglCopyBuffers");
	egl::CreateContext = libHandle.GetFunction<PROC_EGL_eglCreateContext>("eglCreateContext");
	egl::CreatePbufferSurface = libHandle.GetFunction<PROC_EGL_eglCreatePbufferSurface>("eglCreatePbufferSurface");
	egl::CreatePixmapSurface = libHandle.GetFunction<PROC_EGL_eglCreatePixmapSurface>("eglCreatePixmapSurface");
	egl::CreateWindowSurface = libHandle.GetFunction<PROC_EGL_eglCreateWindowSurface>("eglCreateWindowSurface");
	egl::DestroyContext = libHandle.GetFunction<PROC_EGL_eglDestroyContext>("eglDestroyContext");
	egl::DestroySurface = libHandle.GetFunction<PROC_EGL_eglDestroySurface>("eglDestroySurface");
	egl::GetConfigAttrib = libHandle.GetFunction<PROC_EGL_eglGetConfigAttrib>("eglGetConfigAttrib");
	egl::GetConfigs = libHandle.GetFunction<PROC_EGL_eglGetConfigs>("eglGetConfigs");
	egl::GetCurrentContext = libHandle.GetFunction<PROC_EGL_eglGetCurrentContext>("eglGetCurrentContext");
	egl::GetCurrentDisplay = libHandle.GetFunction<PROC_EGL_eglGetCurrentDisplay>("eglGetCurrentDisplay");
	egl::GetCurrentSurface = libHandle.GetFunction<PROC_EGL_eglGetCurrentSurface>("eglGetCurrentSurface");
	egl::GetDisplay = libHandle.GetFunction<PROC_EGL_eglGetDisplay>("eglGetDisplay");
	egl::GetError = libHandle.GetFunction<PROC_EGL_eglGetError>("eglGetError");
	egl::GetProcAddress = libHandle.GetFunction<PROC_EGL_eglGetProcAddress>("eglGetProcAddress");
	egl::Initialize = libHandle.GetFunction<PROC_EGL_eglInitialize>("eglInitialize");
	egl::MakeCurrent = libHandle.GetFunction<PROC_EGL_eglMakeCurrent>("eglMakeCurrent");
	egl::QueryContext = libHandle.GetFunction<PROC_EGL_eglQueryContext>("eglQueryContext");
	egl::QueryString = libHandle.GetFunction<PROC_EGL_eglQueryString>("eglQueryString");
	egl::QuerySurface = libHandle.GetFunction<PROC_EGL_eglQuerySurface>("eglQuerySurface");
	egl::SwapBuffers = libHandle.GetFunction<PROC_EGL_eglSwapBuffers>("eglSwapBuffers");
	egl::Terminate = libHandle.GetFunction<PROC_EGL_eglTerminate>("eglTerminate");
	egl::WaitGL = libHandle.GetFunction<PROC_EGL_eglWaitGL>("eglWaitGL");
	egl::WaitNative = libHandle.GetFunction<PROC_EGL_eglWaitNative>("eglWaitNative");
	egl::BindTexImage = libHandle.GetFunction<PROC_EGL_eglBindTexImage>("eglBindTexImage");
	egl::ReleaseTexImage = libHandle.GetFunction<PROC_EGL_eglReleaseTexImage>("eglReleaseTexImage");
	egl::SurfaceAttrib = libHandle.GetFunction<PROC_EGL_eglSurfaceAttrib>("eglSurfaceAttrib");
	egl::SwapInterval = libHandle.GetFunction<PROC_EGL_eglSwapInterval>("eglSwapInterval");
	egl::BindAPI = libHandle.GetFunction<PROC_EGL_eglBindAPI>("eglBindAPI");
	egl::CreatePbufferFromClientBuffer = libHandle.GetFunction<PROC_EGL_eglCreatePbufferFromClientBuffer>("eglCreatePbufferFromClientBuffer");
	egl::QueryAPI = libHandle.GetFunction<PROC_EGL_eglQueryAPI>("eglQueryAPI");
	egl::ReleaseThread = libHandle.GetFunction<PROC_EGL_eglReleaseThread>("eglReleaseThread");
	egl::WaitClient = libHandle.GetFunction<PROC_EGL_eglWaitClient>("eglWaitClient");

	return true;
}

PROC_EGL_eglChooseConfig egl::ChooseConfig = nullptr;
PROC_EGL_eglCopyBuffers egl::CopyBuffers = nullptr;
PROC_EGL_eglCreateContext egl::CreateContext = nullptr;
PROC_EGL_eglCreatePbufferSurface egl::CreatePbufferSurface = nullptr;
PROC_EGL_eglCreatePixmapSurface egl::CreatePixmapSurface = nullptr;
PROC_EGL_eglCreateWindowSurface egl::CreateWindowSurface = nullptr;
PROC_EGL_eglDestroyContext egl::DestroyContext = nullptr;
PROC_EGL_eglDestroySurface egl::DestroySurface = nullptr;
PROC_EGL_eglGetConfigAttrib egl::GetConfigAttrib = nullptr;
PROC_EGL_eglGetConfigs egl::GetConfigs = nullptr;
PROC_EGL_eglGetCurrentContext egl::GetCurrentContext = nullptr;
PROC_EGL_eglGetCurrentDisplay egl::GetCurrentDisplay = nullptr;
PROC_EGL_eglGetCurrentSurface egl::GetCurrentSurface = nullptr;
PROC_EGL_eglGetDisplay egl::GetDisplay = nullptr;
PROC_EGL_eglGetError egl::GetError = nullptr;
PROC_EGL_eglGetProcAddress egl::GetProcAddress = nullptr;
PROC_EGL_eglInitialize egl::Initialize = nullptr;
PROC_EGL_eglMakeCurrent egl::MakeCurrent = nullptr;
PROC_EGL_eglQueryContext egl::QueryContext = nullptr;
PROC_EGL_eglQueryString egl::QueryString = nullptr;
PROC_EGL_eglQuerySurface egl::QuerySurface = nullptr;
PROC_EGL_eglSwapBuffers egl::SwapBuffers = nullptr;
PROC_EGL_eglTerminate egl::Terminate = nullptr;
PROC_EGL_eglWaitGL egl::WaitGL = nullptr;
PROC_EGL_eglWaitNative egl::WaitNative = nullptr;
PROC_EGL_eglBindTexImage egl::BindTexImage = nullptr;
PROC_EGL_eglReleaseTexImage egl::ReleaseTexImage = nullptr;
PROC_EGL_eglSurfaceAttrib egl::SurfaceAttrib = nullptr;
PROC_EGL_eglSwapInterval egl::SwapInterval = nullptr;
PROC_EGL_eglBindAPI egl::BindAPI = nullptr;
PROC_EGL_eglCreatePbufferFromClientBuffer egl::CreatePbufferFromClientBuffer = nullptr;
PROC_EGL_eglQueryAPI egl::QueryAPI = nullptr;
PROC_EGL_eglReleaseThread egl::ReleaseThread = nullptr;
PROC_EGL_eglWaitClient egl::WaitClient = nullptr;

bool eglext::InitExtensions(const char* const extensions) {
	if (extensions == nullptr) {
		return false;
	}

	// EGL_ANDROID_swap_rectangle
	supports_EGL_ANDROID_swap_rectangle = isExtensionSupported(extensions, "EGL_ANDROID_swap_rectangle");
	eglext::SetSwapRectangleANDROID = reinterpret_cast<PROC_EGL_eglSetSwapRectangleANDROID>(egl::GetProcAddress("eglSetSwapRectangleANDROID"));
	// EGL_ANDROID_get_render_buffer
	supports_EGL_ANDROID_get_render_buffer = isExtensionSupported(extensions, "EGL_ANDROID_get_render_buffer");
	eglext::GetRenderBufferANDROID = reinterpret_cast<PROC_EGL_eglGetRenderBufferANDROID>(egl::GetProcAddress("eglGetRenderBufferANDROID"));

	// EGL_KHR_lock_surface
	supports_EGL_KHR_lock_surface = isExtensionSupported(extensions, "EGL_KHR_lock_surface");
	eglext::LockSurfaceKHR = reinterpret_cast<PROC_EGL_eglLockSurfaceKHR>(egl::GetProcAddress("eglLockSurfaceKHR"));
	eglext::UnlockSurfaceKHR = reinterpret_cast<PROC_EGL_eglUnlockSurfaceKHR>(egl::GetProcAddress("eglUnlockSurfaceKHR"));

	return true;
}

bool eglext::supports_EGL_ANDROID_swap_rectangle = false;
bool eglext::supports_EGL_ANDROID_get_render_buffer = false;
bool eglext::supports_EGL_KHR_lock_surface = false;

PROC_EGL_eglSetSwapRectangleANDROID eglext::SetSwapRectangleANDROID = nullptr;
PROC_EGL_eglGetRenderBufferANDROID eglext::GetRenderBufferANDROID = nullptr;
PROC_EGL_eglLockSurfaceKHR eglext::LockSurfaceKHR = nullptr;
PROC_EGL_eglUnlockSurfaceKHR eglext::UnlockSurfaceKHR = nullptr;