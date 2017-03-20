#pragma once

namespace tinygles
{
class Application
{
public:
	virtual ~Application();

	virtual void InitApplication() = 0;
	virtual void InitView() = 0;
	virtual void RenderFrame() = 0;
	virtual void ReleaseView() = 0;
	virtual void ReleaseApplication() = 0;
};

extern "C" Application * CreateApplication();
} // namespace tinygles