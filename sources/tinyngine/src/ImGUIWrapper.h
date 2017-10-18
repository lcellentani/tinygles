#pragma once

#include "PlatformDefine.h"
#include "imgui.h"

namespace tinyngine
{

class ImGUIWrapper {
public:
	virtual ~ImGUIWrapper() = default;
	virtual void BeginFrame(int32_t windowWidth, int32_t windowHeight) = 0;
	virtual void EndFrame() = 0;
};

std::unique_ptr<ImGUIWrapper> CreateImGUIWrapper();

} // namespace tinyngine