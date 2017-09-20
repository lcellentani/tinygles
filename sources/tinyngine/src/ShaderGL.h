#pragma once

#include "GLApi.h"

namespace tinyngine
{

class ShaderGL {
public:
	ShaderGL() : mId(0), mType(0) {}

	void Create(GLenum type, const char* source);
	void Destroy();

	inline GLint GetId() const { return mId; }

private:
	GLuint mId;
	GLenum mType;
};

} // namespace tinyngine