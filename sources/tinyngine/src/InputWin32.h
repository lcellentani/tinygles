#pragma once

#include "Input.h"

namespace tinyngine
{

class InputWin32 : public Input {
public:
	virtual ~InputWin32() = default;

	void SetMousePosition(uint32_t x, uint32_t y) override;
	void SetMouseButtonState(MouseButton::Enum button, uint8_t state) override;

	void GetMouseState(MouseState& mouseState) override;
private:

};

} // namespace tinyngine