#include "StopWatchWin32.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace tinyngine
{

struct StopWatchWin32::Impl{
	Impl(): mStartTime(), mEndTime(), mDiffTime(0.0f), mFrequency(0.0) {
		LARGE_INTEGER temp;
		QueryPerformanceFrequency((LARGE_INTEGER*)&temp);
		mFrequency = ((double)temp.QuadPart) / 1000.; // millis
	}

	LARGE_INTEGER mStartTime;
	LARGE_INTEGER mEndTime;
	float mDiffTime;
	double  mFrequency;
};

StopWatchWin32::StopWatchWin32() : mImpl(new Impl()) {

}

void StopWatchWin32::Start() {
	QueryPerformanceCounter((LARGE_INTEGER*)&mImpl->mStartTime);
	mRunning = true;
}

void StopWatchWin32::Stop() {
	QueryPerformanceCounter((LARGE_INTEGER*)&mImpl->mEndTime);
	mImpl->mDiffTime = (float)(((double)mImpl->mEndTime.QuadPart - (double)mImpl->mStartTime.QuadPart) / mImpl->mFrequency);
	mRunning = false;
}

void StopWatchWin32::Reset() {
	mImpl->mDiffTime = 0;
	if (mRunning) {
		QueryPerformanceCounter((LARGE_INTEGER*)&mImpl->mStartTime);
	}
}

float StopWatchWin32::GetTime() const {
	if (mRunning) {
		LARGE_INTEGER temp;
		QueryPerformanceCounter((LARGE_INTEGER*)&temp);
		return (float)(((double)temp.QuadPart - (double)mImpl->mStartTime.QuadPart) / mImpl->mFrequency);
	}
	return mImpl->mDiffTime;
}

} // namespace tinyngine