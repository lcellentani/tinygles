#pragma once
/*
//#include "PlatformDefine.h"
#include "IPlatformBridge.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

//namespace tinyngine { class IPlatformContext;  }
//namespace tinyngine { class Application; }

namespace tinyngine
{
class PlatformBridgeWin32 : public IPlatformBridge {
public:
	PlatformBridgeWin32() = delete;
	explicit PlatformBridgeWin32(HWND hwnd);
	virtual ~PlatformBridgeWin32() = default;

	Event PollEvents() override;

private:
	HWND mHwnd;

private:
	void CreateNativeWindow();
	void ReshapeNativeWindow(uint32_t width, uint32_t height, bool windowFrame);
	void ClearNativeWindow();

private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam);

private:
	IPlatformContext* mPlatformContext = nullptr;
	Application* mApplication;

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
};

} // namespace tinyngine
*/