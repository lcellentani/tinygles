#include "GeometryUtil.h"
#include "Log.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include "tiny_obj_loader.h"

namespace tinyngine
{

void GenerateCube(float scale, CubeGeometry& result) {
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

void LoadObj(const  char * filename, bool triangulate) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, nullptr, triangulate);
	if (ret) {
		Log(Logger::Information, "# of vertices  : %d", (attrib.vertices.size() / 3));
		Log(Logger::Information, "# of normals  : %d", (attrib.normals.size() / 3));
		Log(Logger::Information, "# of texcoords  : %d", (attrib.texcoords.size() / 2));
		Log(Logger::Information, "# of shapes  : %d", shapes.size());
		Log(Logger::Information, "# of materials  : %d", materials.size());

		for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
			Log(Logger::Information, "  v[%ld] = (%f, %f, %f)", static_cast<long>(v),
				static_cast<const double>(attrib.vertices[3 * v + 0]),
				static_cast<const double>(attrib.vertices[3 * v + 1]),
				static_cast<const double>(attrib.vertices[3 * v + 2]));
		}
		for (size_t v = 0; v < attrib.normals.size() / 3; v++) {
			Log(Logger::Information, "  n[%ld] = (%f, %f, %f)", static_cast<long>(v),
				static_cast<const double>(attrib.normals[3 * v + 0]),
				static_cast<const double>(attrib.normals[3 * v + 1]),
				static_cast<const double>(attrib.normals[3 * v + 2]));
		}
		for (size_t v = 0; v < attrib.texcoords.size() / 2; v++) {
			Log(Logger::Information, "  uv[%ld] = (%f, %f)", static_cast<long>(v),
				static_cast<const double>(attrib.texcoords[2 * v + 0]),
				static_cast<const double>(attrib.texcoords[2 * v + 1]));
		}

		for (size_t i = 0; i < shapes.size(); i++) {
			Log(Logger::Information, "shape[%ld].name = %s", static_cast<long>(i), shapes[i].name.c_str());
			Log(Logger::Information, "Size of shape[%ld].indices: %lu", static_cast<long>(i), static_cast<unsigned long>(shapes[i].mesh.indices.size()));
			Log(Logger::Information, "shape[%ld].num_faces: %lu", static_cast<long>(i), static_cast<unsigned long>(shapes[i].mesh.num_face_vertices.size()));

			size_t index_offset = 0;
			// For each face
			for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
				size_t fnum = shapes[i].mesh.num_face_vertices[f];
				Log(Logger::Information, "  face[%ld].fnum = %ld", static_cast<long>(f), static_cast<unsigned long>(fnum));

				// For each vertex in the face
				for (size_t v = 0; v < fnum; v++) {
					tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
					Log(Logger::Information, "    face[%ld].v[%ld].idx = %d/%d/%d", static_cast<long>(f), static_cast<long>(v), idx.vertex_index, idx.normal_index, idx.texcoord_index);
				}
				Log(Logger::Information, "  face[%ld].material_id = %d", static_cast<long>(f), shapes[i].mesh.material_ids[f]);

				index_offset += fnum;
			}

			/*Log(Logger::Information, "shape[%ld].num_tags: %lu\n", static_cast<long>(i), static_cast<unsigned long>(shapes[i].mesh.tags.size()));
			for (size_t t = 0; t < shapes[i].mesh.tags.size(); t++) {
				printf("  tag[%ld] = %s ", static_cast<long>(t),
					shapes[i].mesh.tags[t].name.c_str());
				printf(" ints: [");
				for (size_t j = 0; j < shapes[i].mesh.tags[t].intValues.size(); ++j) {
					printf("%ld", static_cast<long>(shapes[i].mesh.tags[t].intValues[j]));
					if (j < (shapes[i].mesh.tags[t].intValues.size() - 1)) {
						printf(", ");
					}
				}
				printf("]");

				printf(" floats: [");
				for (size_t j = 0; j < shapes[i].mesh.tags[t].floatValues.size(); ++j) {
					printf("%f", static_cast<const double>(
						shapes[i].mesh.tags[t].floatValues[j]));
					if (j < (shapes[i].mesh.tags[t].floatValues.size() - 1)) {
						printf(", ");
					}
				}
				printf("]");

				printf(" strings: [");
				for (size_t j = 0; j < shapes[i].mesh.tags[t].stringValues.size(); ++j) {
					printf("%s", shapes[i].mesh.tags[t].stringValues[j].c_str());
					if (j < (shapes[i].mesh.tags[t].stringValues.size() - 1)) {
						printf(", ");
					}
				}
				printf("]");
				printf("\n");
			}*/
		}
	}
}

} // namespace tinyngine