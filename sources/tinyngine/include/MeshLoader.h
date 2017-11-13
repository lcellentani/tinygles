#pragma once

#include "System.h"
#include <cstdint>
#include <vector>

namespace tinyngine
{

struct MeshInfo {
	uint32_t mNumVertices;
	std::vector<float> mPositions; // stride = 3
	std::vector<float> mNormals; // stride = 3;
	std::vector<float> mTexcoords; // stride = 2

	uint32_t mNumIndices;
	std::vector<uint32_t> mIndices;
};

class MeshLoader final : public System  {
public:
	MeshInfo GenerateCube(float scale);

	std::vector<MeshInfo> LoadObj(const char* filename, bool triangulate = true);
};

} // namespace tinyngine