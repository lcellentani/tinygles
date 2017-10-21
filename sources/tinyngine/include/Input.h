#pragma once

#include "System.h"
#include "PlatformTypes.h"

namespace tinyngine
{

class Input : public System {
public:
	virtual ~Input() = default;

	virtual void SetMousePosition(uint32_t x, uint32_t y) = 0;
	virtual void SetMouseButtonState(MouseButton::Enum button, uint8_t state) = 0;
	
	virtual void GetMouseState(MouseState& mouseState) = 0;

};

} // namespace tinyngine