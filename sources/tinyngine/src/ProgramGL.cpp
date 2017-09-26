#include "ProgramGL.h"
#include "VertexFormat.h"

#include <memory>
#include <algorithm>

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

	std::memset(mPredefinedUniforms, 0, sizeof(mPredefinedUniforms));
	for (GLint n = 0; n < activeUniforms; n++) {
		GLint size;
		GLenum type = 0;
		
		GL_CHECK(glGetActiveUniform(mId, n, maxLength + 1, NULL, &size, &type, attribName));
		GLint location = glGetUniformLocation(mId, attribName);
		GL_ERROR(location == -1);

		GLint uniform = -1;
		for (GLint i = 0; i < Uniforms::Count && uniform == -1; i++) {
			if (std::strncmp(tinyngine::gl::GetPredefinedUniformName(static_cast<Uniforms::Enum>(i)), attribName, maxLength) == 0) {
				mPredefinedUniforms[i].mLocation = location;
				mPredefinedUniforms[i].mSize = static_cast<uint16_t>(size);
				mPredefinedUniforms[i].mType = static_cast<uint8_t>(type);
				uniform = i;
			}
		}
	}

	std::memset(mAttributeLocations, -1, sizeof(mAttributeLocations));
	std::memset(mUsedAttributes, UINT8_MAX, sizeof(mUsedAttributes));
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

const Uniforms& ProgramGL::GetUniform(Uniforms::Enum uniform) const {
	return mPredefinedUniforms[uniform];
}

} // namespace tinyngine