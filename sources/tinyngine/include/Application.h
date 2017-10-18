#pragma once

#include "PlatformTypes.h"
#include "Renderer.h"

namespace tinyngine
{
class Application
{
public:
	virtual ~Application() = default;

	virtual ContextAttribs& GetContextAttribs() = 0;

	virtual void InitApplication() = 0;
	virtual void InitView(std::unique_ptr<Renderer>& renderer, uint32_t windowWidth, uint32_t windowHeight) = 0;
	virtual void RenderFrame(std::unique_ptr<Renderer>& renderer) = 0;
	virtual void ReleaseView(std::unique_ptr<Renderer>& renderer) = 0;
	virtual void ReleaseApplication() = 0;
};

extern "C" Application * CreateApplication();
} // namespace tinyngine