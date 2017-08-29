#pragma once

#include <cstdint>

namespace tinygles
{

uint32_t GenerateCube(float scale, float **vertices, float **normals, float **texCoords, uint32_t **indices);

} // namespace tinygles