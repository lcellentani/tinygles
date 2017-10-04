#include "GeometryUtil.h"
#include "Log.h"

#include <cstring>
#include <algorithm>

#include <fstream>
#include <iostream>
#include "tiny_obj_loader.h"

namespace tinyngine
{

void GenerateCube(float scale, Geometry& result) {
	const float cubeVerts[] = {
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f,  0.5f, 0.5f,
		0.5f,  0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
	};

	const float cubeNormals[] = {
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};

	const float cubeTex[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};

	uint32_t cubeIndices[] = {
		0, 2, 1,
		0, 3, 2,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 15, 14,
		12, 14, 13,
		16, 17, 18,
		16, 18, 19,
		20, 23, 22,
		20, 22, 21
	};
	constexpr int numVertices = 24;
	constexpr int numIndices = 36;

	result.numVertices = numVertices;
	result.numIndices = numIndices;

	result.positions.reserve(numVertices);
	for (uint32_t i = 0; i < numVertices * 3; i++) {
		result.positions.push_back(cubeVerts[i] * scale);
	}
	result.normals.reserve(numVertices);
	for (uint32_t i = 0; i < numVertices * 3; i++) {
		result.normals.push_back(cubeNormals[i]);
	}
	result.texcoords.reserve(numVertices);
	for (uint32_t i = 0; i < numVertices * 2; i++) {
		result.texcoords.push_back(cubeTex[i]);
	}

	result.numIndices = numIndices;
	result.indices.reserve(numIndices);
	for (uint32_t i = 0; i < numIndices; i++) {
		result.indices.push_back(cubeIndices[i]);
	}
}

void LoadObj(const  char * filename, bool triangulate, ObjGeometry& result) {
	result.numShapes = 0;
	result.shapes.clear();
	
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, nullptr, triangulate);
	if (ret) {
		for (size_t i = 0; i < shapes.size(); i++) {
			std::vector<tinyobj::index_t> vertices;
			Geometry newGeometry{ 0 };

			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
				size_t fnum = shapes[i].mesh.num_face_vertices[f];
				for (size_t v = 0; v < fnum; v++) {
					tinyobj::index_t thisIdx = shapes[i].mesh.indices[index_offset + v];
					auto elem = std::find_if(std::begin(vertices), std::end(vertices), [&thisIdx](const tinyobj::index_t otherIdx) {
						return thisIdx.vertex_index == otherIdx.vertex_index && thisIdx.normal_index == otherIdx.normal_index && thisIdx.texcoord_index == otherIdx.texcoord_index;
					});
					if (elem == vertices.end()) {
						vertices.push_back(thisIdx);
					}
				}
				index_offset += fnum;
			}
			
			index_offset = 0;
			for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
				size_t fnum = shapes[i].mesh.num_face_vertices[f];
				for (size_t v = 0; v < fnum; v++) {
					tinyobj::index_t thisIdx = shapes[i].mesh.indices[index_offset + v];
					for (size_t n = 0; n < vertices.size(); n++) {
						tinyobj::index_t otherIdx = vertices[n];
						if (thisIdx.vertex_index == otherIdx.vertex_index && thisIdx.normal_index == otherIdx.normal_index && thisIdx.texcoord_index == otherIdx.texcoord_index) {
							newGeometry.indices.push_back(n);
							newGeometry.numIndices++;
							break;
						}
					}
				}
				index_offset += fnum;
			}

			int32_t numVertices = attrib.vertices.size() / 3;
			int32_t numNormals = attrib.normals.size() / 3;
			int32_t numTexcoords = attrib.texcoords.size() / 2;
			for(size_t ii = 0; ii < vertices.size(); ii++) {
				tinyobj::index_t idx = vertices[ii];
				if (numVertices > 0 && idx.vertex_index < numVertices) {
					newGeometry.positions.push_back(attrib.vertices[3 * idx.vertex_index]);
					newGeometry.positions.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
					newGeometry.positions.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
				}
				if (numNormals > 0 && idx.normal_index < numNormals) {
					newGeometry.normals.push_back(attrib.normals[3 * idx.normal_index]);
					newGeometry.normals.push_back(attrib.normals[3 * idx.normal_index + 1]);
					newGeometry.normals.push_back(attrib.normals[3 * idx.normal_index + 2]);
				}
				if (numTexcoords > 0 && idx.texcoord_index < numTexcoords) {
					newGeometry.texcoords.push_back(attrib.texcoords[2 * idx.texcoord_index]);
					newGeometry.texcoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
				}
				newGeometry.numVertices++;
			}

			result.shapes.push_back(newGeometry);
			result.numShapes++;
		}

		/*Log(Logger::Information, "# of shapes: %d", result.numShapes);
		for (auto& shape : result.shapes) {
			Log(Logger::Information, "# of vertices  : %d", shape.positions.size());
			Log(Logger::Information, "# of normals  : %d", shape.normals.size());
			Log(Logger::Information, "# of texcoords  : %d", shape.texcoords.size());
			for (size_t ii = 0; ii < shape.positions.size(); ii += 3) {
				Log(Logger::Information, "v[%d]: %f, %f, %f", (ii / 3), shape.positions[ii], shape.positions[ii + 1], shape.positions[ii + 2]);
			}

			Log(Logger::Information, "# of indexes  : %d", shape.indices.size());
			for (size_t ii = 0; ii < shape.indices.size(); ii++) {
				Log(Logger::Information, "i[%d]: %d", ii, shape.indices[ii]);
			}
		}*/
	}
}

} // namespace tinyngine