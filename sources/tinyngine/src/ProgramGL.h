#pragma once

#include "GraphicsTypes.h"
#include "GLApi.h"
#include "ShaderGL.h"
#include "VertexFormat.h"
#include <array>
#include <vector>

namespace tinyngine
{

class ProgramGL {
public:
	ProgramGL() : mId(0) {}

	void Create(const ShaderGL& vs, const ShaderGL& fs);
	void Initialize();

	void BindAttributes(const VertexFormat& vertexFormat, const std::array<GLuint, Attributes::Count>& handles);
	void BindAttributes(const VertexFormat& vertexFormat, uint32_t baseVertex);
	void UnbindAttributes();

	UniformHandle GetUniformHandle(const char* uniformName) const;

	void SetUniformFloat3(const UniformHandle& uniformHandle, const float* data);
	void SetUniformMat4(const UniformHandle& uniformHandle, const float* data, bool transpose);

	inline GLint GetId() const { return mId; }
	inline bool IsValid() const { return mId > 0; }

private:
	GLint mId;
	std::array<GLint, Attributes::Count> mAttributeLocations;
	//GLint mAttributeLocations[Attributes::Count];
	uint16_t mUsedAttributesCount;
	std::array<uint8_t, Attributes::Count> mUsedAttributes;
	//uint8_t mUsedAttributes[Attributes::Count];
	std::vector<Uniform> mUniforms;
	//Uniforms mPredefinedUniforms[PredefinedUniforms::Count];
};

} // namespace tinyngine