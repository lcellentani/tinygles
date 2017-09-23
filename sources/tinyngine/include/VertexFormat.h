#pragma once

#include "GraphicsTypes.h"

namespace tinyngine
{

struct VertexFormat final {
	VertexFormat();

	void Add(Attributes::Enum attrib, AttributeType::Enum type, uint8_t componentsCount, bool normalized);

	void Decode(Attributes::Enum attrib, uint8_t& type, uint8_t& componentCounts, bool& normalized) const;

	bool IsValid(Attributes::Enum attrib) const { return mAttributes[attrib] != UINT16_MAX; }

	uint16_t mAttributes[Attributes::Count];
	uint16_t mOffset[Attributes::Count];
	uint16_t mStride;
};

} // namespace tinyngine