#pragma once

#include <cstdint>
#include <vector>

namespace tinyngine
{

struct Geometry {
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
void LoadObj(const  char * filename, bool triangulate, ObjGeometry& result);

} // namespace tinyngine