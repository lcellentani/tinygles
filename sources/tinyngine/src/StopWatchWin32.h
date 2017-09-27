#pragma once
#include "StopWatch.h"

namespace tinyngine
{

class StopWatchWin32 : public StopWatch {
public:
	StopWatchWin32();
	virtual ~StopWatchWin32() = default;

	void Start() override;

	void Stop() override;

	void Reset() override;

	float GetTime() const override;

private:
	struct Impl;
	Impl* mImpl;
};

} // namespace tinyngine