#include "ProgramGL.h"
#include <memory>
#include <algorithm>

namespace {

static const char* cAttributeNames[] =
{
	"a_position",
	"a_normal",
	"a_tangent",
	"a_bitangent",
	"a_color0",
	"a_color1",
	"a_texcoord0",
	"a_texcoord1",
	"a_texcoord2",
	"a_texcoord3"
};

static const char* cPredefinedUniformName[] =
{
	"u_view",
	"u_proj",
	"u_model",
	"u_viewProj",
	"u_modelView",
	"u_modelViewProj"
};

}

namespace tinyngine
{

void ProgramGL::Create(const ShaderGL& vs, const ShaderGL& fs) {
	mId = glCreateProgram();

	glAttachShader(mId, vs.GetId());
	if (fs.GetId()) {
		glAttachShader(mId, fs.GetId());
	}

	glLinkProgram(mId);

	GLint isLinked;
	glGetProgramiv(mId, GL_LINK_STATUS, &isLinked);
	if (!isLinked) {
		int infoLogLength;
		glGetProgramiv(mId, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 1) {
			int charactersWritten;
			char* infoLog = new char[infoLogLength];
			glGetProgramInfoLog(mId, infoLogLength, &charactersWritten, infoLog);
			// write somewhere
			delete[] infoLog;
		}
		glDeleteProgram(mId);
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
	glGetProgramiv(mId, GL_ACTIVE_ATTRIBUTES, &activeAttribs);
	glGetProgramiv(mId, GL_ACTIVE_UNIFORMS, &activeUniforms);
	glGetProgramiv(mId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribLength);
	glGetProgramiv(mId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLength);

	GLint maxLength = std::max(maxAttribLength, maxUniformLength);
	char* attribName = new char[maxLength + 1];

	std::memset(mPredefinedUniforms, 0, sizeof(mPredefinedUniforms));
	for (GLint n = 0; n < activeUniforms; n++) {
		GLint size;
		GLenum type = 0;
		
		glGetActiveUniform(mId, n, maxLength + 1, NULL, &size, &type, attribName);
		GLint loc = glGetUniformLocation(mId, attribName);

		GLint uniform = -1;
		for (GLint i = 0; i < Uniforms::Count && uniform == -1; i++) {
			if (std::strncmp(cPredefinedUniformName[i], attribName, maxLength) == 0) {
				mPredefinedUniforms[i].mLocation = loc;
				mPredefinedUniforms[i].mSize = static_cast<uint16_t>(size);
				mPredefinedUniforms[i].mType = static_cast<uint8_t>(type);
				uniform = i;
			}
		}
	}

	std::memset(mAttributes, -1, sizeof(mAttributes));
	std::memset(mUsedAttributes, 0xff, sizeof(mUsedAttributes));
	uint16_t used = 0;
	for (uint8_t n = 0; n < Attributes::Count; n++) {
		GLint loc = glGetAttribLocation(mId, cAttributeNames[n]);
		if (loc >= 0) {
			mAttributes[n] = loc;
			mUsedAttributes[used++] = n;
		}
	}
	mUsedAttributesCount = used;

	delete attribName;
}

void ProgramGL::BindAttributes() {
}

void ProgramGL::UnbindAttributes() {

}

const Uniforms& ProgramGL::GetUniform(Uniforms::Enum uniform) const {
	return mPredefinedUniforms[uniform];
}

} // namespace tinyngine