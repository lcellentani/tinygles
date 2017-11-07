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
};
/*struct Geometry {
	uint32_t numVertices;
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> texcoords;

	uint32_t numIndices;
	std::vector<uint32_t> indices;
};

struct ObjGeometry {
	uint32_t numShapes;
	std::vector<Geometry> shapes;
};

void GenerateCube(float scale, Geometry& result);
void LoadObj(const  char * filename, bool triangulate, ObjGeometry& result);*/

} // namespace tinyngine