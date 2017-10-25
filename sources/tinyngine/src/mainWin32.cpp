#include "IPlatformBridge.h"
#include "IPlatformContext.h"
#include "Application.h"
#include "Engine.h"
#include "Input.h"
#include "Time.h"
#include "Renderer.h"
#include "ImGUIWrapper.h"
#include "Log.h"
#include "imgui.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <windowsx.h>
#include <xinput.h>

#include <thread>

namespace
{

static constexpr uint32_t cDefaultWidth = 1280;
static constexpr uint32_t cDefaultHeight = 720;

struct TranslateKeyModifiers {
	int mVirtualKey;
	tinyngine::Modifier::Enum mModifier;
};

const TranslateKeyModifiers cTranslateKeyModifiers[8]{
	{ VK_LMENU, tinyngine::Modifier::LeftAlt },
	{ VK_RMENU, tinyngine::Modifier::RightAlt },
	{ VK_LCONTROL, tinyngine::Modifier::LeftCtrl },
	{ VK_RCONTROL, tinyngine::Modifier::RightCtrl },
	{ VK_LSHIFT, tinyngine::Modifier::LeftShift },
	{ VK_RSHIFT, tinyngine::Modifier::RightShift },
	{ VK_LWIN, tinyngine::Modifier::LeftMeta },
	{ VK_RWIN, tinyngine::Modifier::RightMeta },
};

uint8_t cTranslateKey[256]{ 0 };

static tinyngine::Key::Enum translateKey(WPARAM wparam) {
	return (tinyngine::Key::Enum)cTranslateKey[wparam & 0xff];
}

static uint8_t translateKeyModifiers() {
	uint8_t modifiers = 0;
	for (auto& entry : cTranslateKeyModifiers) {
		modifiers |= 0 > GetKeyState(entry.mVirtualKey) ? entry.mModifier : tinyngine::Modifier::None;
	}
	return modifiers;
}

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
		mEventQueue = std::make_unique<EventQueue>();

		cTranslateKey[VK_ESCAPE] = Key::Esc;
		cTranslateKey[VK_RETURN] = Key::Return;
		cTranslateKey[VK_TAB] = Key::Tab;
		cTranslateKey[VK_BACK] = Key::Backspace;
		cTranslateKey[VK_SPACE] = Key::Space;
		cTranslateKey[VK_UP] = Key::Up;
		cTranslateKey[VK_DOWN] = Key::Down;
		cTranslateKey[VK_LEFT] = Key::Left;
		cTranslateKey[VK_RIGHT] = Key::Right;
		cTranslateKey[VK_INSERT] = Key::Insert;
		cTranslateKey[VK_DELETE] = Key::Delete;
		cTranslateKey[VK_HOME] = Key::Home;
		cTranslateKey[VK_END] = Key::End;
		cTranslateKey[VK_PRIOR] = Key::PageUp;
		cTranslateKey[VK_NEXT] = Key::PageDown;
		cTranslateKey[VK_SNAPSHOT] = Key::Print;
		cTranslateKey[VK_OEM_PLUS] = Key::Plus;
		cTranslateKey[VK_OEM_MINUS] = Key::Minus;
		cTranslateKey[VK_OEM_4] = Key::LeftBracket;
		cTranslateKey[VK_OEM_6] = Key::RightBracket;
		cTranslateKey[VK_OEM_1] = Key::Semicolon;
		cTranslateKey[VK_OEM_7] = Key::Quote;
		cTranslateKey[VK_OEM_COMMA] = Key::Comma;
		cTranslateKey[VK_OEM_PERIOD] = Key::Period;
		cTranslateKey[VK_DECIMAL] = Key::Period;
		cTranslateKey[VK_OEM_2] = Key::Slash;
		cTranslateKey[VK_OEM_5] = Key::Backslash;
		cTranslateKey[VK_OEM_3] = Key::Tilde;
		cTranslateKey[VK_F1] = Key::F1;
		cTranslateKey[VK_F2] = Key::F2;
		cTranslateKey[VK_F3] = Key::F3;
		cTranslateKey[VK_F4] = Key::F4;
		cTranslateKey[VK_F5] = Key::F5;
		cTranslateKey[VK_F6] = Key::F6;
		cTranslateKey[VK_F7] = Key::F7;
		cTranslateKey[VK_F8] = Key::F8;
		cTranslateKey[VK_F9] = Key::F9;
		cTranslateKey[VK_F10] = Key::F10;
		cTranslateKey[VK_F11] = Key::F11;
		cTranslateKey[VK_F12] = Key::F12;
		cTranslateKey[VK_NUMPAD0] = Key::NumPad0;
		cTranslateKey[VK_NUMPAD1] = Key::NumPad1;
		cTranslateKey[VK_NUMPAD2] = Key::NumPad2;
		cTranslateKey[VK_NUMPAD3] = Key::NumPad3;
		cTranslateKey[VK_NUMPAD4] = Key::NumPad4;
		cTranslateKey[VK_NUMPAD5] = Key::NumPad5;
		cTranslateKey[VK_NUMPAD6] = Key::NumPad6;
		cTranslateKey[VK_NUMPAD7] = Key::NumPad7;
		cTranslateKey[VK_NUMPAD8] = Key::NumPad8;
		cTranslateKey[VK_NUMPAD9] = Key::NumPad9;
		cTranslateKey[uint8_t('0')] = Key::Key0;
		cTranslateKey[uint8_t('1')] = Key::Key1;
		cTranslateKey[uint8_t('2')] = Key::Key2;
		cTranslateKey[uint8_t('3')] = Key::Key3;
		cTranslateKey[uint8_t('4')] = Key::Key4;
		cTranslateKey[uint8_t('5')] = Key::Key5;
		cTranslateKey[uint8_t('6')] = Key::Key6;
		cTranslateKey[uint8_t('7')] = Key::Key7;
		cTranslateKey[uint8_t('8')] = Key::Key8;
		cTranslateKey[uint8_t('9')] = Key::Key9;
		cTranslateKey[uint8_t('A')] = Key::KeyA;
		cTranslateKey[uint8_t('B')] = Key::KeyB;
		cTranslateKey[uint8_t('C')] = Key::KeyC;
		cTranslateKey[uint8_t('D')] = Key::KeyD;
		cTranslateKey[uint8_t('E')] = Key::KeyE;
		cTranslateKey[uint8_t('F')] = Key::KeyF;
		cTranslateKey[uint8_t('G')] = Key::KeyG;
		cTranslateKey[uint8_t('H')] = Key::KeyH;
		cTranslateKey[uint8_t('I')] = Key::KeyI;
		cTranslateKey[uint8_t('J')] = Key::KeyJ;
		cTranslateKey[uint8_t('K')] = Key::KeyK;
		cTranslateKey[uint8_t('L')] = Key::KeyL;
		cTranslateKey[uint8_t('M')] = Key::KeyM;
		cTranslateKey[uint8_t('N')] = Key::KeyN;
		cTranslateKey[uint8_t('O')] = Key::KeyO;
		cTranslateKey[uint8_t('P')] = Key::KeyP;
		cTranslateKey[uint8_t('Q')] = Key::KeyQ;
		cTranslateKey[uint8_t('R')] = Key::KeyR;
		cTranslateKey[uint8_t('S')] = Key::KeyS;
		cTranslateKey[uint8_t('T')] = Key::KeyT;
		cTranslateKey[uint8_t('U')] = Key::KeyU;
		cTranslateKey[uint8_t('V')] = Key::KeyV;
		cTranslateKey[uint8_t('W')] = Key::KeyW;
		cTranslateKey[uint8_t('X')] = Key::KeyX;
		cTranslateKey[uint8_t('Y')] = Key::KeyY;
		cTranslateKey[uint8_t('Z')] = Key::KeyZ;
	}

	std::unique_ptr<Event> PollEvents() override {
		return mEventQueue->poll();
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
		wnd.lpszClassName = "tinyngineclass";
		wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		RegisterClassExA(&wnd);

		mHwnd = CreateWindow("tinyngineclass", "Tinyngine", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, cDefaultWidth, cDefaultHeight, NULL, NULL, instance, this);

		ReshapeNativeWindow(cDefaultWidth, cDefaultHeight, true);
		ClearNativeWindow();

		std::thread mainThred(&MainThread::Func, this);
		mainThred.detach();

		mEventQueue->postSizeEvent(mWidth, mHeight);

		std::chrono::milliseconds delay(50);
		MSG msg{ 0 };
		while (!mExitRequired) {
			std::this_thread::sleep_for(delay);
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
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
		} else {
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
		mFrameWidth = (newrect.right - newrect.left) - prewidth;
		mFrameHeight = (newrect.bottom - newrect.top) - preheight;

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
	uint32_t mFrameWidth;
	uint32_t mFrameHeight;

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
	} else {
		thisClass = reinterpret_cast<PlatformBridgeWin32*>(GetWindowLongPtr(hwnd, GWL_USERDATA));
	}

	switch (id) {
	case WM_QUIT:
	case WM_CLOSE:
		if (hwnd == thisClass->mHwnd) {
			thisClass->mEventQueue->postExitEvent();
		}
		return 0;
	case WM_SIZING:
		break;
	case WM_SIZE:
		break;
	case WM_MOUSEMOVE:
		{
			int32_t mx = GET_X_LPARAM(lparam);
			int32_t my = GET_Y_LPARAM(lparam);

			if (0 == mx && 0 == my) {
				break;
			}

			//setMousePos(_hwnd, m_mx, m_my);
			thisClass->mEventQueue->postMouseEvent(mx, my, MouseButton::None, 0);
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
		{
			uint8_t pressed = (id == WM_LBUTTONDOWN) ? 1 : 0;
			if (pressed) {
				SetCapture(thisClass->mHwnd);
			} else {
				ReleaseCapture();
			}
			thisClass->mEventQueue->postMouseEvent(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), MouseButton::Left, pressed);
		}
		break;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
		{
			uint8_t pressed = (id == WM_LBUTTONDOWN) ? 1 : 0;
			if (pressed) {
				SetCapture(thisClass->mHwnd);
			}
			else {
				ReleaseCapture();
			}
			thisClass->mEventQueue->postMouseEvent(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), MouseButton::Middle, pressed);
		}
		break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
		{
			uint8_t pressed = (id == WM_LBUTTONDOWN) ? 1 : 0;
			if (pressed) {
				SetCapture(thisClass->mHwnd);
			}
			else {
				ReleaseCapture();
			}
			thisClass->mEventQueue->postMouseEvent(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), MouseButton::Right, pressed);
		}
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		{
			uint8_t modifiers = translateKeyModifiers();
			Key::Enum key = translateKey(wparam);

			if (Key::Print == key && 0x3 == ((uint32_t)(lparam) >> 30)) {

				thisClass->mEventQueue->postKeyEvent(key, modifiers, true);
			}

			thisClass->mEventQueue->postKeyEvent(key, modifiers, id == WM_KEYDOWN || id == WM_SYSKEYDOWN);
		}
		break;
	case WM_CHAR:
		{
			std::array<uint8_t, 4> utf8{ 0 };
			uint8_t len = (uint8_t)WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)&wparam, 1, (LPSTR)&utf8[0], 4, NULL, NULL);
			if (len) {
				thisClass->mEventQueue->postCharEvent((uint32_t)wparam, utf8, len);
			}
		}
		break;
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
	
	std::unique_ptr<Engine> engine = std::make_unique<Engine>();
	Renderer& renderer = engine->GetSystem<Renderer>();
	Input& input = engine->GetSystem<Input>();
	ImGUIWrapper& uiWrapper = engine->GetSystem<ImGUIWrapper>();

	pinst->mApplication->InitView((*engine), pinst->mWidth, pinst->mHeight);

	MouseState mouseState{ 0 };
	do {
		std::unique_ptr<Event> event = pinst->PollEvents();
		if (event) {
			switch (event->mType) {
			case Event::Exit:
				pinst->mExitRequired = true;
				break;
			case Event::Size:
				if (SizeEvent* sizeEvent = reinterpret_cast<SizeEvent*>(event.get())) {
					renderer.SetViewport(0, 0, sizeEvent->mWidth, sizeEvent->mHeight);
				}
				break;
			case Event::Key:
				if (KeyEvent* keyEvent = reinterpret_cast<KeyEvent*>(event.get())) {
					Log(Logger::Information, "key: %d, pressed = %d, modifiers = %d", keyEvent->mKey, keyEvent->mPressed, keyEvent->mModifilers);
				}
				break;
			case Event::Char:
				if (CharEvent* charEvent = reinterpret_cast<CharEvent*>(event.get())) {
					//Log(Logger::Information, "char: %s", (char*)&charEvent->mChar[0]);
					uiWrapper.AddInputCharacter(charEvent->mCodepoint);
				}
				break;
			case Event::Mouse:
				if (MouseEvent* mouseEvent = reinterpret_cast<MouseEvent*>(event.get())) {
					//Log(Logger::Information, "x = %d , y = %d - button =  %d - pressed = %d", mouseEvent->mPosX, mouseEvent->mPosY, mouseEvent->mButton, mouseEvent->mPressed);
					input.SetMousePosition(mouseEvent->mPosX, mouseEvent->mPosY);
					input.SetMouseButtonState(mouseEvent->mButton, mouseEvent->mPressed);
				}
				break;
			}
		}

		input.GetMouseState(mouseState);

		uiWrapper.BeginFrame(mouseState, pinst->mWidth, pinst->mHeight);

		pinst->mApplication->RenderFrame((*engine));

		uiWrapper.EndFrame();

		pinst->mPlatformContext->Present();
	} while (!pinst->mExitRequired);

	pinst->mApplication->ReleaseView((*engine));

	engine.reset();

	pinst->mPlatformContext->Terminate();
	pinst->mApplication->ReleaseApplication();
}


} // namespace tinyngine

int main(int argc, char** argv) {
	tinyngine::PlatformBridgeWin32* platformInstance = new tinyngine::PlatformBridgeWin32();
	return platformInstance->Run(argc, argv);
}
