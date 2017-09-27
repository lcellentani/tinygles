#pragma once

namespace tinyngine
{

class StopWatch {
public:
	StopWatch() = default;
	virtual ~StopWatch() = default;

	virtual void Start() = 0;

	virtual void Stop() = 0;

	virtual void Reset() = 0;

	virtual float GetTime() const = 0;

	bool IsRunning() const { return mRunning; }

protected:
	bool mRunning = false;
};

} // namespace tinyngine