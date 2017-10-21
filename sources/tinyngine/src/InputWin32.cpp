#include "InputWin32.h"

namespace tinyngine
{

void InputWin32::SetMousePosition(uint32_t x, uint32_t y) {
	TINYNGINE_UNUSED(x); TINYNGINE_UNUSED(y);
}

void InputWin32::SetMouseButtonState(MouseButton::Enum button, uint8_t state) {
	TINYNGINE_UNUSED(button); TINYNGINE_UNUSED(state);
}

void InputWin32::GetMouseState(MouseState& mouseState) {
	TINYNGINE_UNUSED(mouseState);
}

} // tinyngine