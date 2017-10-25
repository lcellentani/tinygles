#pragma once

#include "PlatformDefine.h"
#include "PlatformTypes.h"
#include "System.h"

namespace tinyngine
{

class ImGUIWrapper : public System {
public:
	virtual ~ImGUIWrapper() = default;
	virtual void BeginFrame(MouseState& mouseState, int32_t windowWidth, int32_t windowHeight) = 0;
	virtual void EndFrame() = 0;

	virtual void AddInputCharacter(uint32_t ch) = 0;
};

ImGUIWrapper* CreateImGUIWrapper();

} // namespace tinyngine