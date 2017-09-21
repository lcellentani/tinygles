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

	inline bool IsValid() const { return mId > 0; }

private:
	GLuint mId;
	GLenum mType;
};

} // namespace tinyngine