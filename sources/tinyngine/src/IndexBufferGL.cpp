#include "IndexBufferGL.h"
#include "PlatformDefine.h"

namespace tinyngine
{

void IndexBufferGL::Create(const void* data, uint32_t size) {
	glGenBuffers(1, &mId);
	GL_ERROR(mId == 0);
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId));
	GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER , size , data, (data == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW)));
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

} // namespace tinyngine