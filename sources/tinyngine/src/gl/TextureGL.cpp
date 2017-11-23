#include "TextureGL.h"
#include "PlatformDefine.h"

namespace
{

static void CreateTextureImage(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* data) {
	glTexImage2D(target, level, internalFormat, width, height, border, format, type, data);
}

}

namespace tinyngine
{

void TextureGL::Create(GLenum target, const ImageData& imageData, TextureFormats::Enum textureFormat) {
	mTarget = target;

	GL_CHECK(glGenTextures(1, &mId));
	GL_ERROR(mId == 0);
	GL_CHECK(glBindTexture(mTarget, mId));
	GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
}

void TextureGL::Destroy() {
	if (mId > 0) {
		GL_CHECK(glBindTexture(mTarget, mId));
		GL_CHECK(glDeleteTextures(1, &mId));
		mId = 0;
	}
}

void TextureGL::Bind() {
	if (mId > 0) {
		GL_CHECK(glBindTexture(mTarget, mId));
	}
}

} // namespace tinyngine