#pragma once

#include "System.h"

namespace tinyngine
{

class Input : public System {
public:
	virtual ~Input() = default;

	virtual int GetI() = 0;
	virtual void SetI(int) = 0;
};

} // namespace tinyngine