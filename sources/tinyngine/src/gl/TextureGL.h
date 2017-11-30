#pragma once

#include "GLApi.h"
#include "ImageLoader.h"

namespace tinyngine
{

class TextureGL {
public:
	TextureGL() = default;

	void Create(GLenum target, const ImageHandle& imageHandle, ImagesManager& imagesManager, TextureFormats::Enum textureFormat, TextureFilteringMode::Enum filtering, bool useMipmaps);
	void Destroy();
	void Bind(uint32_t stage);

	void SetFilteringMode(TextureFilteringMode::Enum filtering);
	void SetWrappingMode(TextureWrapMode::Enum wrapS, TextureWrapMode::Enum wrapT);

	inline GLint GetId() const { return mId; }
	inline GLenum GetTarget() const { return mTarget; }

	inline bool IsValid() const { return mId > 0; }

private:
	GLuint mId = 0;
	GLenum mTarget = 0;
	GLsizei mWidth = 0;
	GLsizei mHeight = 0;
	GLboolean mUseMipmaps = false;
};

} // namespace tinyngine