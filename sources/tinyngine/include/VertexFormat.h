#pragma once

#include "GraphicsTypes.h"

namespace tinyngine
{

struct VertexFormat final {
	VertexFormat();

	void Add(Attributes::Enum attrib, AttributeType::Enum type, uint8_t componentsCount, bool normalized);

	uint16_t mAttributes[Attributes::Count];
	uint16_t mOffset[Attributes::Count];
	uint16_t mStride;
};

} // namespace tinyngine