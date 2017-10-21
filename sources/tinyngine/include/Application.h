#pragma once

#include "PlatformTypes.h"
#include "Engine.h"

namespace tinyngine
{
class Application
{
public:
	virtual ~Application() = default;

	virtual ContextAttribs& GetContextAttribs() = 0;

	virtual void InitApplication() = 0;
	virtual void InitView(Engine& engine, uint32_t windowWidth, uint32_t windowHeight) = 0;
	virtual void RenderFrame(Engine& engine) = 0;
	virtual void ReleaseView(Engine& engine) = 0;
	virtual void ReleaseApplication() = 0;
};

extern "C" Application * CreateApplication();
} // namespace tinyngine