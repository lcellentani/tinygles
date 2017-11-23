#pragma once

#include "GLApi.h"

namespace tinyngine
{

class TextureGL {
public:
	TextureGL() = default;

	void Create(GLenum target, const ImageData& imageData, TextureFormats::Enum textureFormat);
	void Destroy();
	void Bind(uint32_t stage);

	inline GLint GetId() const { return mId; }
	inline GLenum GetTarget() const { return mTarget; }

	inline bool IsValid() const { return mId > 0; }

private:
	GLuint mId = 0;
	GLenum mTarget = 0;
	GLsizei mWidth = 0;
	GLsizei mHeight = 0;
};

} // namespace tinyngine