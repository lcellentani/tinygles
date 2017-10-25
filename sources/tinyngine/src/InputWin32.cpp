#include "InputWin32.h"

namespace tinyngine
{

InputWin32::InputWin32() {
	memset(mMousePosition, 0, sizeof(mMousePosition));
	memset(mMouseButtons, MouseButton::None, sizeof(mMouseButtons));
}

void InputWin32::SetMousePosition(int32_t x, int32_t y) {
	mMousePosition[0] = x;
	mMousePosition[1] = y;
}

void InputWin32::SetMouseButtonState(MouseButton::Enum button, uint8_t state) {
	mMouseButtons[button] = state;
}

void InputWin32::GetMouseState(MouseState& mouseState) {
	memcpy(mouseState.mButtons, mMouseButtons, sizeof(MouseButton));
	mouseState.mPosX = mMousePosition[0];
	mouseState.mPosY = mMousePosition[1];
}

} // tinyngine