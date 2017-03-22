#pragma once

#include "EGLApi.h"

#ifndef TARGET_OS_IPHONE
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglChooseConfig)(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglCopyBuffers)(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target);
typedef EGLContext(EGLAPIENTRY* PROC_EGL_eglCreateContext)(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list);

typedef EGLSurface(EGLAPIENTRY* PROC_EGL_eglCreatePbufferSurface)(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list);
typedef EGLSurface(EGLAPIENTRY* PROC_EGL_eglCreatePixmapSurface)(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list);
typedef EGLSurface(EGLAPIENTRY* PROC_EGL_eglCreateWindowSurface)(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglDestroyContext)(EGLDisplay dpy, EGLContext ctx);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglDestroySurface)(EGLDisplay dpy, EGLSurface surface);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglGetConfigAttrib)(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglGetConfigs)(EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config);
typedef EGLContext(EGLAPIENTRY* PROC_EGL_eglGetCurrentContext)(void);
typedef EGLDisplay(EGLAPIENTRY* PROC_EGL_eglGetCurrentDisplay)(void);
typedef EGLSurface(EGLAPIENTRY* PROC_EGL_eglGetCurrentSurface)(EGLint readdraw);
typedef EGLDisplay(EGLAPIENTRY* PROC_EGL_eglGetDisplay)(EGLNativeDisplayType display_id);

typedef EGLint(EGLAPIENTRY* PROC_EGL_eglGetError)(void);
typedef __eglMustCastToProperFunctionPointerType(EGLAPIENTRY* PROC_EGL_eglGetProcAddress)(const char *procname);

typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglInitialize)(EGLDisplay dpy, EGLint *major, EGLint *minor);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglMakeCurrent)(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglQueryContext)(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value);
typedef const char* (EGLAPIENTRY* PROC_EGL_eglQueryString)(EGLDisplay dpy, EGLint name);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglQuerySurface)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value);

typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglTerminate)(EGLDisplay dpy);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglWaitGL)(void);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglWaitNative)(EGLint engine);

typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglBindTexImage)(EGLDisplay dpy, EGLSurface surface, EGLint buffer);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglReleaseTexImage)(EGLDisplay dpy, EGLSurface surface, EGLint buffer);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglSurfaceAttrib)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglSwapInterval)(EGLDisplay dpy, EGLint interval);

typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglBindAPI)(EGLenum api);
typedef EGLSurface(EGLAPIENTRY* PROC_EGL_eglCreatePbufferFromClientBuffer)(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list);
typedef EGLenum(EGLAPIENTRY* PROC_EGL_eglQueryAPI)(void);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglReleaseThread)(void);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglWaitClient)(void);

// EGL extensions
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglSetSwapRectangleANDROID)(EGLDisplay dpy, EGLSurface draw, EGLint left, EGLint top, EGLint width, EGLint height);
typedef EGLClientBuffer(EGLAPIENTRY* PROC_EGL_eglGetRenderBufferANDROID)(EGLDisplay dpy, EGLSurface draw);

// KHR_lock_surface
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglLockSurfaceKHR)(EGLDisplay display, EGLSurface surface, const EGLint* attrib_list);
typedef EGLBoolean(EGLAPIENTRY* PROC_EGL_eglUnlockSurfaceKHR)(EGLDisplay display, EGLSurface surface);
#endif

class egl {
public:
	static bool InitTrampoline();

public:
	static PROC_EGL_eglChooseConfig ChooseConfig;
	static PROC_EGL_eglCopyBuffers CopyBuffers;
	static PROC_EGL_eglCreateContext CreateContext;

	static PROC_EGL_eglCreatePbufferSurface CreatePbufferSurface;
	static PROC_EGL_eglCreatePixmapSurface CreatePixmapSurface;
	static PROC_EGL_eglCreateWindowSurface CreateWindowSurface;
	static PROC_EGL_eglDestroyContext DestroyContext;
	static PROC_EGL_eglDestroySurface DestroySurface;
	static PROC_EGL_eglGetConfigAttrib GetConfigAttrib;
	static PROC_EGL_eglGetConfigs GetConfigs;
	static PROC_EGL_eglGetCurrentContext GetCurrentContext;
	static PROC_EGL_eglGetCurrentDisplay GetCurrentDisplay;
	static PROC_EGL_eglGetCurrentSurface GetCurrentSurface;
	static PROC_EGL_eglGetDisplay GetDisplay;
	static PROC_EGL_eglGetError GetError;
	static PROC_EGL_eglGetProcAddress GetProcAddress;
	static PROC_EGL_eglInitialize Initialize;
	static PROC_EGL_eglMakeCurrent MakeCurrent;
	static PROC_EGL_eglQueryContext QueryContext;
	static PROC_EGL_eglQueryString QueryString;
	static PROC_EGL_eglQuerySurface QuerySurface;
	static PROC_EGL_eglSwapBuffers SwapBuffers;
	static PROC_EGL_eglTerminate Terminate;
	static PROC_EGL_eglWaitGL WaitGL;
	static PROC_EGL_eglWaitNative WaitNative;
	static PROC_EGL_eglBindTexImage BindTexImage;
	static PROC_EGL_eglReleaseTexImage ReleaseTexImage;
	static PROC_EGL_eglSurfaceAttrib SurfaceAttrib;
	static PROC_EGL_eglSwapInterval SwapInterval;
	static PROC_EGL_eglBindAPI BindAPI;
	static PROC_EGL_eglCreatePbufferFromClientBuffer CreatePbufferFromClientBuffer;
	static PROC_EGL_eglQueryAPI QueryAPI;
	static PROC_EGL_eglReleaseThread ReleaseThread;
	static PROC_EGL_eglWaitClient WaitClient;
};

class eglext {
public:
	static bool InitExtensions(const char* const extensions);

	// EGL_ANDROID_swap_rectangle
	static bool supports_EGL_ANDROID_swap_rectangle;
	// EGL_ANDROID_get_render_buffer
	static bool supports_EGL_ANDROID_get_render_buffer;
	// EGL_KHR_lock_surface
	static bool supports_EGL_KHR_lock_surface;

public:
	// EGL_ANDROID_swap_rectangle
	static PROC_EGL_eglSetSwapRectangleANDROID SetSwapRectangleANDROID;
	// EGL_ANDROID_get_render_buffer
	static PROC_EGL_eglGetRenderBufferANDROID GetRenderBufferANDROID;

	// EGL_KHR_lock_surface
	static PROC_EGL_eglLockSurfaceKHR LockSurfaceKHR;
	static PROC_EGL_eglUnlockSurfaceKHR UnlockSurfaceKHR;
};