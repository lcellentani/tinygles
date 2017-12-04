#pragma once

#include "Application.h"

class ExampleApp : public tinyngine::Application {
public:
	ExampleApp() = default;
	virtual ~ExampleApp() = default;

	void InitApplication() override;
	void InitView(tinyngine::Engine&, uint32_t windowWidth, uint32_t windowHeight) override;
	void RenderFrame(tinyngine::Engine& engine) override;
	void ReleaseView(tinyngine::Engine& engine) override;
	void ReleaseApplication() override;
};