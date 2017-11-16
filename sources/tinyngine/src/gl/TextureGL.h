#pragma once

#include "GLApi.h"

namespace tinyngine
{

class TextureGL {
public:
	TextureGL() : mId(0), mType(0) {}

	void Create(GLenum type, const ImageData& imageData);
	void Destroy();
	void Bind();

	inline GLint GetId() const { return mId; }
	inline GLenum GetType() const { return mType; }

	inline bool IsValid() const { return mId > 0; }

private:
	GLuint mId;
	GLenum mType;
};

} // namespace tinyngine