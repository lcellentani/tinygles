#pragma once

#include "PlatformDefine.h"
#include "System.h"

namespace tinyngine
{

class ImGUIWrapper : public System {
public:
	virtual ~ImGUIWrapper() = default;
	virtual void BeginFrame(int32_t windowWidth, int32_t windowHeight) = 0;
	virtual void EndFrame() = 0;
};

ImGUIWrapper* CreateImGUIWrapper();

} // namespace tinyngine