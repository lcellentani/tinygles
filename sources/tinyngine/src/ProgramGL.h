#pragma once

#include "GLApi.h"
#include "ShaderGL.h"

namespace tinyngine
{

class ProgramGL {
public:
	ProgramGL() : mId(0) {}

	void Create(const ShaderGL& vs, const ShaderGL& fs);

	inline GLint GetId() const { return mId; }

private:
	GLint mId;
};

} // namespace tinyngine