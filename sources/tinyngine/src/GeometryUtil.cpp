#include "GeometryUtil.h"
#include "Log.h"

#include <cstring>
#include <algorithm>

#include <fstream>
#include <iostream>
#include "tiny_obj_loader.h"

#include "glm/vec3.hpp"
#include "glm/geometric.hpp"

namespace
{

void GenerateNormalsIfNeeded(tinyngine::Geometry& geometry) {
	if (geometry.normals.size() > 0) {
		return;
	}

	geometry.normals.reserve(geometry.positions.size());
	geometry.normals.resize(geometry.positions.size());
	size_t size = geometry.indices.size();
	for (size_t n = 0; n < size; n += 3) {
		uint32_t index0 = geometry.indices[n] * 3;
		uint32_t index1 = geometry.indices[n + 1] * 3;
		uint32_t index2 = geometry.indices[n + 2] * 3;

		glm::vec3 a(geometry.positions[index0], geometry.positions[index0 + 1], geometry.positions[index0 + 2]);
		glm::vec3 b(geometry.positions[index1], geometry.positions[index1 + 1], geometry.positions[index1 + 2]);
		glm::vec3 c(geometry.positions[index2], geometry.positions[index2 + 1], geometry.positions[index2 + 2]);

		glm::vec3 n1 = glm::normalize(b - a);
		glm::vec3 n2 = glm::normalize(c - a);
		glm::vec3 faceNormal = glm::cross(n1, n2);

		geometry.normals[index0] += faceNormal.x; geometry.normals[index0 + 1] += faceNormal.y; geometry.normals[index0 + 2] += faceNormal.z;
		geometry.normals[index1] += faceNormal.x; geometry.normals[index1 + 1] += faceNormal.y; geometry.normals[index1 + 2] += faceNormal.z;
		geometry.normals[index2] += faceNormal.x; geometry.normals[index2 + 1] += faceNormal.y; geometry.normals[index2 + 2] += faceNormal.z;
 	}

	for (size_t n = 0; n < geometry.normals.size(); n += 3) {
		glm::vec3 a(geometry.normals[n], geometry.normals[n + 1], geometry.normals[n + 2]);
		a = glm::normalize(a);
		geometry.normals[n] = a.x;
		geometry.normals[n + 1] = a.y;
		geometry.normals[n + 2] = a.z;
	}
}

}

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

			//for (auto& shape : result.shapes) {
			{
				auto& shape = newGeometry;
				Log(Logger::Information, "# of vertices  : %d", shape.positions.size());
				Log(Logger::Information, "# of normals  : %d", shape.normals.size());
				Log(Logger::Information, "# of texcoords  : %d", shape.texcoords.size());
				for (size_t ii = 0; ii < shape.positions.size(); ii += 3) {
					if (shape.positions.size() > 0 && shape.normals.size() > 0) {
						Log(Logger::Information, "v[%d]: (%f, %f, %f) - (%f, %f, %f)",
							(ii / 3),
							shape.positions[ii], shape.positions[ii + 1], shape.positions[ii + 2],
							shape.normals[ii], shape.normals[ii + 1], shape.normals[ii + 2]);
					}
					else if (shape.positions.size() > 0) {
						Log(Logger::Information, "v[%d]: (%f, %f, %f) - (%f, %f, %f)",
							(ii / 3),
							shape.positions[ii], shape.positions[ii + 1], shape.positions[ii + 2]);
					}
				}

				//Log(Logger::Information, "# of indexes  : %d", shape.indices.size());
				//for (size_t ii = 0; ii < shape.indices.size(); ii++) {
				//	Log(Logger::Information, "i[%d]: %d", ii, shape.indices[ii]);
				//}
				Log(Logger::Information, "---------------------------------------------------------------------------------");
			}
			//}

			newGeometry.normals.clear();

			GenerateNormalsIfNeeded(newGeometry);

			{
				auto& shape = newGeometry;
				Log(Logger::Information, "# of vertices  : %d", shape.positions.size());
				Log(Logger::Information, "# of normals  : %d", shape.normals.size());
				Log(Logger::Information, "# of texcoords  : %d", shape.texcoords.size());
				for (size_t ii = 0; ii < shape.positions.size(); ii += 3) {
					Log(Logger::Information, "v[%d]: (%f, %f, %f) - (%f, %f, %f)",
						(ii / 3),
						shape.positions[ii], shape.positions[ii + 1], shape.positions[ii + 2],
						shape.normals[ii], shape.normals[ii + 1], shape.normals[ii + 2]);
				}

				//Log(Logger::Information, "# of indexes  : %d", shape.indices.size());
				//for (size_t ii = 0; ii < shape.indices.size(); ii++) {
				//	Log(Logger::Information, "i[%d]: %d", ii, shape.indices[ii]);
				//}
				Log(Logger::Information, "---------------------------------------------------------------------------------");
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