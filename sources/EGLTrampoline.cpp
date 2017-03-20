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

PROC_EGL_eglChooseConfig egl::ChooseConfig;
PROC_EGL_eglCopyBuffers egl::CopyBuffers;
PROC_EGL_eglCreateContext egl::CreateContext;
PROC_EGL_eglCreatePbufferSurface egl::CreatePbufferSurface;
PROC_EGL_eglCreatePixmapSurface egl::CreatePixmapSurface;
PROC_EGL_eglCreateWindowSurface egl::CreateWindowSurface;
PROC_EGL_eglDestroyContext egl::DestroyContext;
PROC_EGL_eglDestroySurface egl::DestroySurface;
PROC_EGL_eglGetConfigAttrib egl::GetConfigAttrib;
PROC_EGL_eglGetConfigs egl::GetConfigs;
PROC_EGL_eglGetCurrentContext egl::GetCurrentContext;
PROC_EGL_eglGetCurrentDisplay egl::GetCurrentDisplay;
PROC_EGL_eglGetCurrentSurface egl::GetCurrentSurface;
PROC_EGL_eglGetDisplay egl::GetDisplay;
PROC_EGL_eglGetError egl::GetError;
PROC_EGL_eglGetProcAddress egl::GetProcAddress;
PROC_EGL_eglInitialize egl::Initialize;
PROC_EGL_eglMakeCurrent egl::MakeCurrent;
PROC_EGL_eglQueryContext egl::QueryContext;
PROC_EGL_eglQueryString egl::QueryString;
PROC_EGL_eglQuerySurface egl::QuerySurface;
PROC_EGL_eglSwapBuffers egl::SwapBuffers;
PROC_EGL_eglTerminate egl::Terminate;
PROC_EGL_eglWaitGL egl::WaitGL;
PROC_EGL_eglWaitNative egl::WaitNative;
PROC_EGL_eglBindTexImage egl::BindTexImage;
PROC_EGL_eglReleaseTexImage egl::ReleaseTexImage;
PROC_EGL_eglSurfaceAttrib egl::SurfaceAttrib;
PROC_EGL_eglSwapInterval egl::SwapInterval;
PROC_EGL_eglBindAPI egl::BindAPI;
PROC_EGL_eglCreatePbufferFromClientBuffer egl::CreatePbufferFromClientBuffer;
PROC_EGL_eglQueryAPI egl::QueryAPI;
PROC_EGL_eglReleaseThread egl::ReleaseThread;
PROC_EGL_eglWaitClient egl::WaitClient;