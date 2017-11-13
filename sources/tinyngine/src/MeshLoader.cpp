#include "MeshLoader.h"
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

void GenerateNormalsIfNeeded(tinyngine::MeshInfo& mesh) {
	if (mesh.mNormals.size() > 0) {
		return;
	}

	mesh.mNormals.reserve(mesh.mPositions.size());
	mesh.mNormals.resize(mesh.mPositions.size());
	size_t size = mesh.mIndices.size();
	for (size_t n = 0; n < size; n += 3) {
		uint32_t index0 = mesh.mIndices[n] * 3;
		uint32_t index1 = mesh.mIndices[n + 1] * 3;
		uint32_t index2 = mesh.mIndices[n + 2] * 3;

		glm::vec3 a(mesh.mPositions[index0], mesh.mPositions[index0 + 1], mesh.mPositions[index0 + 2]);
		glm::vec3 b(mesh.mPositions[index1], mesh.mPositions[index1 + 1], mesh.mPositions[index1 + 2]);
		glm::vec3 c(mesh.mPositions[index2], mesh.mPositions[index2 + 1], mesh.mPositions[index2 + 2]);

		glm::vec3 n1 = glm::normalize(b - a);
		glm::vec3 n2 = glm::normalize(c - a);
		glm::vec3 faceNormal = glm::cross(n1, n2);

		mesh.mNormals[index0] += faceNormal.x; mesh.mNormals[index0 + 1] += faceNormal.y; mesh.mNormals[index0 + 2] += faceNormal.z;
		mesh.mNormals[index1] += faceNormal.x; mesh.mNormals[index1 + 1] += faceNormal.y; mesh.mNormals[index1 + 2] += faceNormal.z;
		mesh.mNormals[index2] += faceNormal.x; mesh.mNormals[index2 + 1] += faceNormal.y; mesh.mNormals[index2 + 2] += faceNormal.z;
	}

	for (size_t n = 0; n < mesh.mNormals.size(); n += 3) {
		glm::vec3 a(mesh.mNormals[n], mesh.mNormals[n + 1], mesh.mNormals[n + 2]);
		a = glm::normalize(a);
		mesh.mNormals[n] = a.x;
		mesh.mNormals[n + 1] = a.y;
		mesh.mNormals[n + 2] = a.z;
	}
}

}

namespace tinyngine
{

MeshInfo MeshLoader::GenerateCube(float scale) {
	static const float cubeVerts[] = {
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

	static const float cubeNormals[] = {
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

	static const float cubeTex[] = {
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

	static const uint32_t cubeIndices[] = {
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

	MeshInfo result;

	result.mNumVertices = numVertices;
	result.mNumIndices = numIndices;

	result.mPositions.reserve(numVertices);
	for (uint32_t i = 0; i < numVertices * 3; i++) {
		result.mPositions.push_back(cubeVerts[i] * scale);
	}
	result.mNormals.reserve(numVertices);
	for (uint32_t i = 0; i < numVertices * 3; i++) {
		result.mNormals.push_back(cubeNormals[i]);
	}
	result.mTexcoords.reserve(numVertices);
	for (uint32_t i = 0; i < numVertices * 2; i++) {
		result.mTexcoords.push_back(cubeTex[i]);
	}

	result.mNumIndices = numIndices;
	result.mIndices.reserve(numIndices);
	for (uint32_t i = 0; i < numIndices; i++) {
		result.mIndices.push_back(cubeIndices[i]);
	}

	return result;
}

std::vector<MeshInfo> MeshLoader::LoadObj(const char* filename, bool triangulate) {
	std::vector<MeshInfo> meshes;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, nullptr, triangulate);
	if (ret) {
		for (size_t i = 0; i < shapes.size(); i++) {
			std::vector<tinyobj::index_t> vertices;
			MeshInfo newMesh{ 0 };

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
							newMesh.mIndices.push_back(n);
							newMesh.mNumIndices++;
							break;
						}
					}
				}
				index_offset += fnum;
			}

			int32_t numVertices = attrib.vertices.size() / 3;
			int32_t numNormals = attrib.normals.size() / 3;
			int32_t numTexcoords = attrib.texcoords.size() / 2;
			for (size_t ii = 0; ii < vertices.size(); ii++) {
				tinyobj::index_t idx = vertices[ii];
				if (numVertices > 0 && idx.vertex_index < numVertices) {
					newMesh.mPositions.push_back(attrib.vertices[3 * idx.vertex_index]);
					newMesh.mPositions.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
					newMesh.mPositions.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
				}
				if (numNormals > 0 && idx.normal_index < numNormals) {
					newMesh.mNormals.push_back(attrib.normals[3 * idx.normal_index]);
					newMesh.mNormals.push_back(attrib.normals[3 * idx.normal_index + 1]);
					newMesh.mNormals.push_back(attrib.normals[3 * idx.normal_index + 2]);
				}
				if (numTexcoords > 0 && idx.texcoord_index < numTexcoords) {
					newMesh.mTexcoords.push_back(attrib.texcoords[2 * idx.texcoord_index]);
					newMesh.mTexcoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
				}
				newMesh.mNumVertices++;
			}

			//for (auto& shape : result.shapes) {
			//{
			//	auto& shape = newMesh;
			//	Log(Logger::Information, "# of vertices  : %d", shape.mPositions.size());
			//	Log(Logger::Information, "# of normals  : %d", shape.normals.size());
			//	Log(Logger::Information, "# of texcoords  : %d", shape.texcoords.size());
			//	for (size_t ii = 0; ii < shape.mPositions.size(); ii += 3) {
			//		if (shape.mPositions.size() > 0 && shape.normals.size() > 0) {
			//			Log(Logger::Information, "v[%d]: (%f, %f, %f) - (%f, %f, %f)",
			//				(ii / 3),
			//				shape.mPositions[ii], shape.mPositions[ii + 1], shape.mPositions[ii + 2],
			//				shape.normals[ii], shape.normals[ii + 1], shape.normals[ii + 2]);
			//		}
			//		else if (shape.mPositions.size() > 0) {
			//			Log(Logger::Information, "v[%d]: (%f, %f, %f) - (%f, %f, %f)",
			//				(ii / 3),
			//				shape.mPositions[ii], shape.mPositions[ii + 1], shape.mPositions[ii + 2]);
			//		}
			//	}
			//	Log(Logger::Information, "# of indexes  : %d", shape.indices.size());
			//	for (size_t ii = 0; ii < shape.indices.size(); ii++) {
			//		Log(Logger::Information, "i[%d]: %d", ii, shape.indices[ii]);
			//	}
			//	Log(Logger::Information, "---------------------------------------------------------------------------------");
			//}
			//}

			//newMesh.normals.clear();

			GenerateNormalsIfNeeded(newMesh);

			//{
			//	auto& shape = newMesh;
			//	Log(Logger::Information, "# of vertices  : %d", shape.mPositions.size());
			//	Log(Logger::Information, "# of normals  : %d", shape.normals.size());
			//	Log(Logger::Information, "# of texcoords  : %d", shape.texcoords.size());
			//	for (size_t ii = 0; ii < shape.mPositions.size(); ii += 3) {
			//		Log(Logger::Information, "v[%d]: (%f, %f, %f) - (%f, %f, %f)", (ii / 3), shape.mPositions[ii], shape.mPositions[ii + 1], shape.mPositions[ii + 2], shape.normals[ii], shape.normals[ii + 1], shape.normals[ii + 2]);
			//	}
			//	Log(Logger::Information, "# of indexes  : %d", shape.indices.size());
			//	for (size_t ii = 0; ii < shape.indices.size(); ii++) {
			//		Log(Logger::Information, "i[%d]: %d", ii, shape.indices[ii]);
			//	}
			//	Log(Logger::Information, "---------------------------------------------------------------------------------");
			//}

			meshes.push_back(newMesh);
		}

		//Log(Logger::Information, "# of shapes: %d", result.numShapes);
		//for (auto& shape : result.shapes) {
		//	Log(Logger::Information, "# of vertices  : %d", shape.mPositions.size());
		//	Log(Logger::Information, "# of normals  : %d", shape.normals.size());
		//	Log(Logger::Information, "# of texcoords  : %d", shape.texcoords.size());
		//	for (size_t ii = 0; ii < shape.mPositions.size(); ii += 3) {
		//		Log(Logger::Information, "v[%d]: %f, %f, %f", (ii / 3), shape.mPositions[ii], shape.mPositions[ii + 1], shape.mPositions[ii + 2]);
		//	}
		//	Log(Logger::Information, "# of indexes  : %d", shape.indices.size());
		//	for (size_t ii = 0; ii < shape.indices.size(); ii++) {
		//		Log(Logger::Information, "i[%d]: %d", ii, shape.indices[ii]);
		//	}
		//}
	}

	return meshes;
}

} // namespace tinyngine
