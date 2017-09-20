#include "ProgramGL.h"

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

} // namespace tinyngine