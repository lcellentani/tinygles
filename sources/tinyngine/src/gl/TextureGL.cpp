#include "TextureGL.h"
#include "PlatformDefine.h"
#include "GLApi.h"

namespace
{

struct TextureFormatInfo {
	GLenum mInternalFormat;
	GLenum mFormat;
	GLenum mType;
};

static TextureFormatInfo sTextureFormats[]{
	{ GL_RGB, GL_RGB, GL_UNSIGNED_BYTE},			// RGB8
	{ GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE },			// RGBA8
	{ GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE }			// BGRA8
};

}

namespace tinyngine
{

void TextureGL::Create(GLenum target, const ImageHandle& imageHandle, ImageManager& imageManager, TextureFormats::Enum textureFormat, TextureFilteringMode::Enum filtering, bool useMipmaps) {
	mTarget = target;
	mUseMipmaps = useMipmaps;

	GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

	GL_CHECK(glGenTextures(1, &mId));
	GL_ERROR(mId == 0);
	GL_CHECK(glBindTexture(mTarget, mId));

	GLenum internalFormat = sTextureFormats[textureFormat].mInternalFormat;
	GLenum format = sTextureFormats[textureFormat].mFormat;
	GLenum type = sTextureFormats[textureFormat].mType;

	ImageData imageData;
	imageManager.GetImageData(imageHandle, 0, imageData);
	GL_CHECK(glTexImage2D(target, 0, internalFormat, imageData.mWidth, imageData.mHeight, 0, format, type, imageData.mData));
	if (mUseMipmaps) {
		uint32_t numMipmaps = imageManager.ImageGetMipmapsCount(imageHandle);
		if (numMipmaps > 1) {
			for (uint32_t level = 1; level < numMipmaps; level++) {
				imageManager.GetImageData(imageHandle, level, imageData);
				GL_CHECK(glTexImage2D(target, 0, internalFormat, imageData.mWidth, imageData.mHeight, 0, format, type, imageData.mData));
			}
		}
		else {
			GL_CHECK(glGenerateMipmap(target));
		}
	}
	GLint magParam = GL_NEAREST;
	GLint minParam = mUseMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
	switch (filtering) {
	case TextureFilteringMode::Enum::Bilinear:
		magParam = GL_LINEAR;
		minParam = mUseMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST;
		break;
	case TextureFilteringMode::Enum::Trilinear:
		magParam = GL_LINEAR;
		minParam = mUseMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
		break;
	default:
		break;
	}
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minParam));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magParam));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GL_CHECK(glBindTexture(mTarget, 0));
}

void TextureGL::Destroy() {
	if (mId > 0) {
		GL_CHECK(glBindTexture(mTarget, mId));
		GL_CHECK(glDeleteTextures(1, &mId));
		mId = 0;
	}
}

void TextureGL::Bind(uint32_t stage) {
	if (mId > 0) {
		GL_CHECK(glActiveTexture(GL_TEXTURE0 + stage));
		GL_CHECK(glBindTexture(mTarget, mId));
	}
}

void TextureGL::SetFilteringMode(TextureFilteringMode::Enum filtering) {
	if (mId > 0) {
		GL_CHECK(glBindTexture(mTarget, mId));
		GLint magParam = GL_NEAREST;
		GLint minParam = mUseMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
		switch (filtering) {
		case TextureFilteringMode::Enum::Bilinear:
			magParam = GL_LINEAR;
			minParam = mUseMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST;
			break;
		case TextureFilteringMode::Enum::Trilinear:
			magParam = GL_LINEAR;
			minParam = mUseMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
			break;
		default:
			break;
		}
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minParam));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magParam));
	}
}

void TextureGL::SetWrappingMode(TextureWrapMode::Enum wrapS, TextureWrapMode::Enum wrapT) {
	if (mId > 0) {
		GL_CHECK(glBindTexture(mTarget, mId));
		GLint paramS = tinyngine::gl::GetWrapMode(wrapS);
		GLint paramT = tinyngine::gl::GetWrapMode(wrapT);
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, paramS));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, paramT));
	}
}

} // namespace tinyngine