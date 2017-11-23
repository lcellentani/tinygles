#include "TextureGL.h"
#include "PlatformDefine.h"

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

void TextureGL::Create(GLenum target, const ImageData& imageData, TextureFormats::Enum textureFormat) {
	mTarget = target;

	GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

	GL_CHECK(glGenTextures(1, &mId));
	GL_ERROR(mId == 0);
	GL_CHECK(glBindTexture(mTarget, mId));
	
	GLenum internalFormat = sTextureFormats[textureFormat].mInternalFormat;
	GLenum format = sTextureFormats[textureFormat].mFormat;
	GLenum type = sTextureFormats[textureFormat].mType;
	GL_CHECK(glTexImage2D(target, 0, internalFormat, imageData.mWidth, imageData.mHeight, 0, format, type, imageData.mData));

	//GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
	//GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	//GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	//GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

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

} // namespace tinyngine