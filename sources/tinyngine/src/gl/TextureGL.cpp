#include "TextureGL.h"
#include "PlatformDefine.h"

namespace tinyngine
{

void TextureGL::Create(GLenum type, const ImageData& imageData) {
	TINYNGINE_UNUSED(type); TINYNGINE_UNUSED(imageData);
}

void TextureGL::Destroy() {
}

void TextureGL::Bind() {
	GL_CHECK(glBindTexture(mType, mId));
}

} // namespace tinyngine