#include "VertexFormat.h"

#include <memory>

namespace {

static const uint8_t cAttribTypeSizeGL[static_cast<uint8_t>(tinyngine::AttributeType::Count)][4] =
{
	{ 1,  2,  4,  4 },
	{ 2,  4,  6,  8 },
	{ 4,  8, 12, 16 } 
};

}

namespace tinyngine
{

VertexFormat::VertexFormat() {
	std::memset(mAttributes, UINT16_MAX, sizeof(mAttributes));
	std::memset(mOffset, 0, sizeof(mOffset));
	mStride = 0;
}

void VertexFormat::Add(Attributes::Enum attrib, AttributeType::Enum type, uint8_t componentsCount, bool normalized) {
	uint8_t iAttrib = static_cast<uint8_t>(attrib);
	uint8_t iType = static_cast<uint8_t>(type);
	uint8_t num = componentsCount - 1;

	const uint16_t encodedCount = num & 3;
	const uint16_t encodedType = (iType & 7) << 3;
	const uint16_t encodedNormalized = (normalized & 1) << 7;
	
	mAttributes[iAttrib] = encodedNormalized | encodedType | encodedCount;
	mOffset[iAttrib] = cAttribTypeSizeGL[iType][num];// mStride;
	mStride += cAttribTypeSizeGL[iType][num];
}

void VertexFormat::Decode(Attributes::Enum attrib, uint8_t& type, uint8_t& componentCounts, bool& normalized) const {
	uint8_t iAttrib = static_cast<uint8_t>(attrib);
	type = (mAttributes[iAttrib] >> 3) & 7;
	componentCounts = (mAttributes[iAttrib] & 3) + 1;
	normalized = (mAttributes[iAttrib] >> 7) & 1;
}

} // namespace tinyngine