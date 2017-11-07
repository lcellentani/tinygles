#include "VertexBufferGL.h"
#include "PlatformDefine.h"

namespace tinyngine
{

void VertexBufferGL::Create(const void* data, uint32_t size, const VertexFormat& vertexFormat) {
	TINYNGINE_UNUSED(vertexFormat);
	glGenBuffers(1, &mId);
	GL_ERROR(mId == UINT32_MAX);
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mId));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, size, data, (data == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW)));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

} // namespace tinyngine