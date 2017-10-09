#include "IPlatformBridge.h"
#include "StopWatchWin32.h"
#include "IPlatformContext.h"
#include "Application.h"
#include "RendererGL.h"
#include "Log.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <thread>

namespace
{

static constexpr uint32_t cDefaultWidth = 800;
static constexpr uint32_t cDefaultHeight = 600;

}

namespace tinyngine
{

class PlatformBridgeWin32;
struct MainThread {
	static void Func(PlatformBridgeWin32* platformInstance);
};

class PlatformBridgeWin32 : public IPlatformBridge {
public:
	PlatformBridgeWin32::PlatformBridgeWin32() {
		mFrameDelta = 0;
		mStopWatch = std::make_unique<StopWatchWin32>();
		mEventQueue = std::make_unique<EventQueue>();
	}

	bool PollEvents(Event* event) override {
		return mEventQueue->poll(event);
	}

	int Run(int argc, char** argv) {
		TINYNGINE_UNUSED(argc); TINYNGINE_UNUSED(argv);

		HINSTANCE instance = (HINSTANCE)GetModuleHandle(NULL);

		WNDCLASSEX wnd;
		memset(&wnd, 0, sizeof(wnd));
		wnd.cbSize = sizeof(wnd);
		wnd.style = CS_HREDRAW | CS_VREDRAW;
		wnd.lpfnWndProc = WndProc;
		wnd.hInstance = instance;
		wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
		wnd.lpszClassName = "esfwkclass";
		wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		RegisterClassExA(&wnd);

		mHwnd = CreateWindow("esfwkclass", "OpenGL ES", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, cDefaultWidth, cDefaultHeight, NULL, NULL, instance, this);

		ReshapeNativeWindow(cDefaultWidth, cDefaultHeight, true);
		ClearNativeWindow();

		std::thread mainThred(&MainThread::Func, this);
		mainThred.detach();

		std::chrono::milliseconds delay(100);
		MSG msg{ 0 };
		while (!mExitRequired) {
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			std::this_thread::sleep_for(delay);
		}

		return 0;
	}

	void ReshapeNativeWindow(uint32_t width, uint32_t height, bool windowFrame) {
		mWidth = width;
		mHeight = height;
		mAspectRatio = float(mWidth) / float(mHeight);

		ShowWindow(mHwnd, SW_SHOWNORMAL);
		RECT rect;
		RECT newrect = { 0, 0, (LONG)width, (LONG)height };
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

		//if (mApplication) {
		//	mApplication->OnReshape(newLeft, newTop, mWidth, mHeight);
		//}
	}

	void ClearNativeWindow() {
		RECT rect;
		GetWindowRect(mHwnd, &rect);
		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
		HDC hdc = GetDC(mHwnd);
		SelectObject(hdc, brush);
		FillRect(hdc, &rect, brush);
	}

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam);

	IPlatformContext* mPlatformContext = nullptr;
	Application* mApplication = nullptr;
	bool mExitRequired = false;

	HWND mHwnd;
	bool mWindowed = true;
	RECT mRect;
	DWORD mStyle;
	uint32_t mWindowWidth;
	uint32_t mWindowHeight;

	uint32_t mWidth;
	uint32_t mHeight;
	float mAspectRatio;
};

LRESULT CALLBACK PlatformBridgeWin32::WndProc(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam) {
	PlatformBridgeWin32* thisClass = nullptr;

	if (id == WM_CREATE) {
		thisClass = static_cast<PlatformBridgeWin32*>(reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams);
		SetLastError(0);
		if (!SetWindowLongPtr(hwnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(thisClass)))
		{
			if (GetLastError() != 0) { return FALSE; }
		}
	}
	else {
		thisClass = reinterpret_cast<PlatformBridgeWin32*>(GetWindowLongPtr(hwnd, GWL_USERDATA));
	}

	switch (id) {
	case WM_QUIT:
	case WM_CLOSE:
		if (hwnd == thisClass->mHwnd) {
			thisClass->mEventQueue->postExitEvent();
		}
		return 0;
	}

	return ::DefWindowProc(hwnd, id, wparam, lparam);
}

void MainThread::Func(tinyngine::PlatformBridgeWin32* pinst) {
	if (!pinst) { return; }
	
	pinst->mApplication = CreateApplication();
	if (!pinst->mApplication) { return; }

	pinst->mPlatformContext = CreatePlatformContext(pinst->mHwnd, pinst->mApplication->GetContextAttribs());
	if (!pinst->mPlatformContext) { return; }

	Result result = pinst->mPlatformContext->Initialize();
	if (result != Result::Success) { return; }
	
	std::unique_ptr<Renderer> renderer = std::make_unique<RendererGL>();

	pinst->mApplication->InitView(renderer);

	pinst->mStopWatch->Start();
	do {
		pinst->mFrameDelta = pinst->mStopWatch->GetTime();
		//pinst->mFrameDelta = (pinst->mFrameDelta + (3.0f / 60.0f)) * 0.25f;
		
		pinst->mApplication->RenderFrame(renderer, pinst->mFrameDelta);
		pinst->mPlatformContext->Present();

		Event event;
		if (pinst->PollEvents(&event)) {
			switch (event.mType) {
			case Event::Exit:
				pinst->mExitRequired = true;
				break;
			}
		}

		pinst->mStopWatch->Reset();
	} while (!pinst->mExitRequired);

	renderer.reset();

	pinst->mPlatformContext->Terminate();
	pinst->mApplication->ReleaseApplication();
}


} // namespace tinyngine

int main(int argc, char** argv) {
	tinyngine::PlatformBridgeWin32* platformInstance = new tinyngine::PlatformBridgeWin32();
	return platformInstance->Run(argc, argv);
}
