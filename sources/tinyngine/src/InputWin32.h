#pragma once

#include "Input.h"

namespace tinyngine
{

class InputWin32 : public Input {
public:
	InputWin32();
	virtual ~InputWin32() = default;

	void SetMousePosition(int32_t x, int32_t y) override;
	void SetMouseButtonState(MouseButton::Enum button, uint8_t state) override;

	void GetMouseState(MouseState& mouseState) override;
private:
	int32_t mMousePosition[2];
	uint8_t mMouseButtons[MouseButton::Count];
};

} // namespace tinyngine