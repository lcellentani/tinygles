#include "Time.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace tinyngine
{

int64_t Time::GetTime() const {
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	int64_t i64 = li.QuadPart;
	return i64;
}

int64_t Time::GetFrequency() const {
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	int64_t i64 = li.QuadPart;
	return i64;
}

} // tinyngine