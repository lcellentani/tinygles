#include "PlatformBridge.h"
#include "Application.h"
#include "IPlatformContext.h"

//#include "make_unique.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace tinygles
{
struct PlatformBridge::Impl
{
	IPlatformContext* mPlatformContext;

	uint32_t mDefaultWidth = 800;
	uint32_t mDefaultHeight = 600;

	uint32_t mWidth;
	uint32_t mHeight;
	float mAspectRatio;
	bool mExitRequired = false;

	bool mWindowed = true;
	HWND mHwnd;
	RECT mRect;
	DWORD mStyle;
	uint32_t mWindowWidth;
	uint32_t mWindowHeight;

	void CreateNativeWindow();
	void ReshapeNativeWindow(uint32_t width, uint32_t height, bool windowFrame);
	void ClearNativeWindow();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam);
};

LRESULT CALLBACK PlatformBridge::Impl::WndProc(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam)
{
	PlatformBridge::Impl* thisClass = nullptr;

	if (id == WM_CREATE) {
		thisClass = static_cast<PlatformBridge::Impl*>(reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams);
		SetLastError(0);
		if (!SetWindowLongPtr(hwnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(thisClass)))
		{
			if (GetLastError() != 0) { return FALSE; }
		}
	}
	else {
		thisClass = reinterpret_cast<PlatformBridge::Impl*>(GetWindowLongPtr(hwnd, GWL_USERDATA));
	}

	switch (id) {
	case WM_QUIT:
	case WM_CLOSE:
		if (hwnd == thisClass->mHwnd) {
			thisClass->mExitRequired = true;
		}
		return 0;
	}

	return ::DefWindowProc(hwnd, id, wparam, lparam);
}

void PlatformBridge::Impl::CreateNativeWindow() {
	HINSTANCE instance = (HINSTANCE)GetModuleHandle(NULL);

	WNDCLASSEX wnd;
	memset(&wnd, 0, sizeof(wnd));
	wnd.cbSize = sizeof(wnd);
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = PlatformBridge::Impl::WndProc;
	wnd.hInstance = instance;
	wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.lpszClassName = "esfwkclass";
	wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassExA(&wnd);

	mHwnd = CreateWindow("esfwkclass", "OpenGL ES", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, mDefaultWidth, mDefaultHeight, NULL, NULL, instance, this);

	ReshapeNativeWindow(mDefaultWidth, mDefaultHeight, true);
	ClearNativeWindow();
}

void PlatformBridge::Impl::ReshapeNativeWindow(uint32_t width, uint32_t height, bool windowFrame) {
	mWidth = width;
	mHeight = height;
	mAspectRatio = float(mWidth) / float(mHeight);

	ShowWindow(mHwnd, SW_SHOWNORMAL);
	RECT rect;
	RECT newrect = { 0, 0, (LONG)mWidth, (LONG)mHeight };
	DWORD style = WS_POPUP | WS_SYSMENU;

	if (mWindowed) {
		GetWindowRect(mHwnd, &mRect);
		mStyle = GetWindowLong(mHwnd, GWL_STYLE);
	}

	if (windowFrame) {
		rect = mRect;
		style = mStyle;
	}
	else {
		HMONITOR monitor = MonitorFromWindow(mHwnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi;
		mi.cbSize = sizeof(mi);
		GetMonitorInfo(monitor, &mi);
		newrect = mi.rcMonitor;
		rect = mi.rcMonitor;
		mAspectRatio = float(newrect.right - newrect.left) / float(newrect.bottom - newrect.top);
	}

	SetWindowLong(mHwnd, GWL_STYLE, style);
	uint32_t prewidth = newrect.right - newrect.left;
	uint32_t preheight = newrect.bottom - newrect.top;
	AdjustWindowRect(&newrect, style, FALSE);
	mWindowWidth = (newrect.right - newrect.left) - prewidth;
	mWindowHeight = (newrect.bottom - newrect.top) - preheight;
	UpdateWindow(mHwnd);

	if (rect.left == -32000 || rect.top == -32000) {
		rect.left = 0;
		rect.top = 0;
	}

	int32_t newLeft = rect.left;
	int32_t newTop = rect.top;
	int32_t newWidth = (newrect.right - newrect.left);
	int32_t newHeight = (newrect.bottom - newrect.top);

	if (!windowFrame) {
		float aspectRatio = 1.0f / mAspectRatio;
		newWidth = width;
		newHeight = uint32_t(float(newWidth)*aspectRatio);

		newLeft = newrect.left + (newrect.right - newrect.left - newWidth) / 2;
		newTop = newrect.top + (newrect.bottom - newrect.top - newHeight) / 2;
	}

	SetWindowPos(mHwnd, HWND_TOP, newLeft, newTop, newWidth, newHeight, SWP_SHOWWINDOW);

	ShowWindow(mHwnd, SW_RESTORE);

	mWindowed = windowFrame;
	if (mWindowed) {
		GetWindowRect(mHwnd, &mRect);
	}
}

void PlatformBridge::Impl::ClearNativeWindow()
{
	RECT rect;
	GetWindowRect(mHwnd, &rect);
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	HDC hdc = GetDC(mHwnd);
	SelectObject(hdc, brush);
	FillRect(hdc, &rect, brush);
}

//=========================================================================================================================================
//=========================================================================================================================================

PlatformBridge::PlatformBridge() : mImpl(std::make_unique<Impl>()) {

}

PlatformBridge::~PlatformBridge() = default;

PlatformBridge::PlatformBridge(PlatformBridge& rhs) : mImpl(nullptr) {
	if (rhs.mImpl != nullptr) {
		mImpl = std::make_unique<Impl>((*rhs.mImpl));
	}
}

PlatformBridge& PlatformBridge::operator=(PlatformBridge& rhs) {
	if (rhs.mImpl == nullptr) {
		mImpl.reset();
	}
	else if (mImpl == nullptr) {
		mImpl = std::make_unique<Impl>((*rhs.mImpl));
	}
	else {
		*mImpl = *rhs.mImpl;
	}
	return (*this);
}

int16_t PlatformBridge::Run() {
	std::unique_ptr<Application> app{ CreateApplication() };

	app->InitApplication();

	mImpl->CreateNativeWindow();

	mImpl->mPlatformContext = CreatePlatformContext(mImpl->mHwnd, app->GetContextAttribs());
	mImpl->mPlatformContext->Initialize();

	app->InitView();

	MSG msg{ 0 };
	while (!mImpl->mExitRequired) {
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		app->RenderFrame();

		mImpl->mPlatformContext->Present();
	}

	app->ReleaseView();
	mImpl->mPlatformContext->Terminate();

	app->ReleaseApplication();

	return 0;
}
} // namespace tinygles