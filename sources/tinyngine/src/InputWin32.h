#pragma once

#include "Input.h"

namespace tinyngine
{

class InputWin32 : public Input {
public:
	virtual ~InputWin32() = default;

	int GetI() override {
		return ii;
	}
	void SetI(int i) override {
		ii = i;
	}

private:
	int ii = 0;
};

} // namespace tinyngine