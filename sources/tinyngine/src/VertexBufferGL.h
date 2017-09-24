#pragma once

#include "GraphicsTypes.h"
#include "GLApi.h"
#include "VertexFormat.h"

namespace tinyngine
{

class VertexBufferGL {
public:
	void Create(const void* data, uint32_t size, const VertexFormat& vertexFormat);

	inline GLint GetId() const { return mId; }

	inline bool IsValid() const { return mId > 0; }

private:
	GLuint mId;
};

} // namespace tinyngine