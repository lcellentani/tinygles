#pragma once

#include "GraphicsTypes.h"

namespace tinyngine
{

struct VertexFormat final {
	VertexFormat();

	void Add(Attributes attrib, AttributeType type, uint8_t componentsCount, bool normalized);

	uint16_t mAttributes[static_cast<uint8_t>(Attributes::Count)];
	uint16_t mOffset[static_cast<uint8_t>(Attributes::Count)];
	uint16_t mStride;
};

} // namespace tinyngine