#include "ProgramGL.h"
#include "VertexFormat.h"

#include <memory>
#include <algorithm>
#include <map>

namespace
{

static std::map<GLenum, tinyngine::UniformType::Enum> cUniformTypeTranslationTable{
	{ GL_FLOAT, tinyngine::UniformType::Float },
	{ GL_FLOAT_VEC2, tinyngine::UniformType::Float2 },
	{ GL_FLOAT_VEC3, tinyngine::UniformType::Float3 },
	{ GL_FLOAT_VEC4, tinyngine::UniformType::Float4 },
	{ GL_INT, tinyngine::UniformType::Int },
	{ GL_INT_VEC2, tinyngine::UniformType::Int2 },
	{ GL_INT_VEC3, tinyngine::UniformType::Int3 },
	{ GL_INT_VEC4, tinyngine::UniformType::Int4 },
	{ GL_BOOL, tinyngine::UniformType::Bool },
	{ GL_BOOL_VEC2, tinyngine::UniformType::Bool2 },
	{ GL_BOOL_VEC3, tinyngine::UniformType::Bool3 },
	{ GL_BOOL_VEC4, tinyngine::UniformType::Bool4 },
	{ GL_FLOAT_MAT2, tinyngine::UniformType::Mat2 },
	{ GL_FLOAT_MAT3, tinyngine::UniformType::Mat3 },
	{ GL_FLOAT_MAT4, tinyngine::UniformType::Mat4 },
	{ GL_SAMPLER_2D, tinyngine::UniformType::Sampler2D },
	{ GL_SAMPLER_CUBE, tinyngine::UniformType::SamplerCube }
};

}

namespace tinyngine
{

void ProgramGL::Create(const ShaderGL& vs, const ShaderGL& fs) {
	mId = glCreateProgram();
	GL_ERROR(mId == 0);
	
	GL_CHECK(glAttachShader(mId, vs.GetId()));
	if (fs.GetId()) {
		GL_CHECK(glAttachShader(mId, fs.GetId()));
	}

	GL_CHECK(glLinkProgram(mId));

	GLint isLinked;
	GL_CHECK(glGetProgramiv(mId, GL_LINK_STATUS, &isLinked));
	if (!isLinked) {
		int infoLogLength;
		GL_CHECK(glGetProgramiv(mId, GL_INFO_LOG_LENGTH, &infoLogLength));
		if (infoLogLength > 1) {
			int charactersWritten;
			char* infoLog = new char[infoLogLength];
			GL_CHECK(glGetProgramInfoLog(mId, infoLogLength, &charactersWritten, infoLog));
			// write somewhere
			delete[] infoLog;
		}
		GL_CHECK(glDeleteProgram(mId));
		mId = 0;
	}
}

void ProgramGL::Initialize() {
	if (!IsValid()) {
		return;
	}

	GLint activeAttribs = 0;
	GLint activeUniforms = 0;
	GLint maxAttribLength = 0;
	GLint maxUniformLength = 0;
	GL_CHECK(glGetProgramiv(mId, GL_ACTIVE_ATTRIBUTES, &activeAttribs));
	GL_CHECK(glGetProgramiv(mId, GL_ACTIVE_UNIFORMS, &activeUniforms));
	GL_CHECK(glGetProgramiv(mId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribLength));
	GL_CHECK(glGetProgramiv(mId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLength));

	GLint maxLength = std::max(maxAttribLength, maxUniformLength);
	char* attribName = new char[maxLength + 1];

	mUniforms.reserve(activeUniforms);
	for (GLint n = 0; n < activeUniforms; n++) {
		GLint size;
		GLenum type = 0;
		
		GL_CHECK(glGetActiveUniform(mId, n, maxLength + 1, NULL, &size, &type, attribName));
		GLint location = glGetUniformLocation(mId, attribName);
		GL_ERROR(location == -1);

		Uniform uniform;
		uniform.mLocation = location;
		uniform.mSize = static_cast<uint16_t>(size);
		uniform.mType = cUniformTypeTranslationTable[type];
		memset(uniform.mName, 0, sizeof(uniform.mName));
		strcpy(uniform.mName, attribName);
		mUniforms.push_back(uniform);
	}

	std::fill(std::begin(mAttributeLocations), std::end(mAttributeLocations), -1);
	std::fill(std::begin(mUsedAttributes), std::end(mUsedAttributes), UINT8_MAX);
	uint16_t used = 0;
	for (uint8_t n = 0; n < Attributes::Count; n++) {
		GLint location = glGetAttribLocation(mId, tinyngine::gl::GetAttributeName(static_cast<Attributes::Enum>(n)));
		GL_ERROR(location == -1);
		if (location >= 0) {
			mAttributeLocations[n] = location;
			mUsedAttributes[used++] = n;
		}
	}
	mUsedAttributesCount = used;

	delete attribName;
}

void ProgramGL::BindAttributes(const VertexFormat& vertexFormat, const std::array<GLuint, Attributes::Count>& handles) {
	for (GLint n = 0; n < mUsedAttributesCount; n++) {
		Attributes::Enum attrib = static_cast<Attributes::Enum>(mUsedAttributes[n]);
		GLint location = mAttributeLocations[attrib];
		if (location >= 0 && vertexFormat.IsValid(attrib)) {
			uint8_t size;
			uint8_t type;
			bool normalized;
			vertexFormat.Decode(attrib, type, size, normalized);

			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, handles[attrib]));

			GL_CHECK(glEnableVertexAttribArray(location));
			GL_CHECK(glVertexAttribPointer(location, size, tinyngine::gl::GetAttributeType(static_cast<AttributeType::Enum>(type)), normalized, vertexFormat.mOffset[attrib], 0));
		}
	}
}

void ProgramGL::BindAttributes(const VertexFormat& vertexFormat, uint32_t baseVertex) {
	for (GLint n = 0; n < mUsedAttributesCount; n++) {
		Attributes::Enum attrib = static_cast<Attributes::Enum>(mUsedAttributes[n]);
		GLint location = mAttributeLocations[attrib];
		if (location >= 0 && vertexFormat.IsValid(attrib)) {
			uint8_t size;
			uint8_t type;
			bool normalized;
			vertexFormat.Decode(attrib, type, size, normalized);

			GL_CHECK(glEnableVertexAttribArray(location));
			uint32_t firstIndex = baseVertex * vertexFormat.mStride + vertexFormat.mOffset[attrib];
			GL_CHECK(glVertexAttribPointer(location, size, tinyngine::gl::GetAttributeType(static_cast<AttributeType::Enum>(type)), normalized, vertexFormat.mStride, (void*)(uintptr_t)firstIndex));
		}
	}
}

void ProgramGL::UnbindAttributes() {
	for (GLint n = 0; n < mUsedAttributesCount; n++) {
		Attributes::Enum attrib = static_cast<Attributes::Enum>(mUsedAttributes[n]);
		GLint location = mAttributeLocations[attrib];
		if (location >= 0) {
			GL_CHECK(glDisableVertexAttribArray(location));
		}
	}
}

UniformHandle ProgramGL::GetUniformHandle(const char* uniformName) const {
	int index = -1;
	for (size_t n = 0; n < mUniforms.size(); n++) {
		if (strcmp(mUniforms[n].mName, uniformName) == 0) {
			index = n;
			break;
		}
	}
	return index != -1 ? UniformHandle(index) : UniformHandle(cInvalidHandle);
}

void ProgramGL::SetUniformInt(const UniformHandle& uniformHandle, int32_t data) {
	auto& uniform = mUniforms[uniformHandle.mHandle];
	GL_CHECK(glUniform1i(uniform.mLocation, data));
}

void ProgramGL::SetUniformFloat(const UniformHandle& uniformHandle, float data) {
	auto& uniform = mUniforms[uniformHandle.mHandle];
	GL_CHECK(glUniform1f(uniform.mLocation, data));
}

void ProgramGL::SetUniformFloat3(const UniformHandle& uniformHandle, const float* data) {
	auto& uniform = mUniforms[uniformHandle.mHandle];
	GL_CHECK(glUniform3fv(uniform.mLocation, uniform.mSize, data));
}

void ProgramGL::SetUniformMat4(const UniformHandle& uniformHandle, const float* data, bool transpose) {
	auto& uniform = mUniforms[uniformHandle.mHandle];
	GL_CHECK(glUniformMatrix4fv(uniform.mLocation, uniform.mSize, transpose ? GL_TRUE : GL_FALSE, data));
}

} // namespace tinyngine