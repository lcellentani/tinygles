#include "Time.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace tinyngine
{

int64_t Time::GetTime() const {
	LARGE_INTEGER li;
	// Performance counter value may unexpectedly leap forward http://support.microsoft.com/kb/274323
	QueryPerformanceCounter(&li);
	int64_t i64 = li.QuadPart;
	return i64;
}

int64_t Time::GetFrequency() const {
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	return li.QuadPart;
}

} // tinyngine