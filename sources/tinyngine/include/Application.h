#pragma once

#include "PlatformTypes.h"
#include "Renderer.h"

namespace tinygles
{
class Application
{
public:
	virtual ~Application();

	virtual ContextAttribs& GetContextAttribs() = 0;

	virtual void InitApplication() = 0;
	virtual void InitView(std::unique_ptr<Renderer>& renderer) = 0;
	virtual void RenderFrame(std::unique_ptr<Renderer>& renderer) = 0;
	virtual void ReleaseView(std::unique_ptr<Renderer>& renderer) = 0;
	virtual void ReleaseApplication() = 0;

	virtual void OnReshape(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { TINYGLES_UNUSED(x); TINYGLES_UNUSED(y); TINYGLES_UNUSED(width); TINYGLES_UNUSED(height); }
};

extern "C" Application * CreateApplication();
} // namespace tinygles