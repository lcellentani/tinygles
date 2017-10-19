#pragma once

#include "System.h"
#include <cstdint>

namespace tinyngine
{

class Time : public System {
public:
	int64_t GetTime() const;

	int64_t GetFrequency() const;
};

} // namespace tinyngine