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

	inline bool IsValid() const { return mId > 0; }

private:
	GLint mId;
};

} // namespace tinyngine