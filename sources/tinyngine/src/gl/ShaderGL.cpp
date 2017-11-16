#include "ShaderGL.h"

namespace tinyngine
{

void ShaderGL::Create(GLenum type, const char* source) {
	if (source != nullptr) {
		mId = glCreateShader(type);
		GL_ERROR(mId == 0);

		GL_CHECK(glShaderSource(mId, 1, (const char**)&source, nullptr));
		GL_CHECK(glCompileShader(mId));

		GLint compileResult;
		GL_CHECK(glGetShaderiv(mId, GL_COMPILE_STATUS, &compileResult));
		if (compileResult == 0) {
			GLint infoLogLength;
			GL_CHECK(glGetShaderiv(mId, GL_INFO_LOG_LENGTH, &infoLogLength));
			if (infoLogLength > 1) {
				char* infoLog = new char[infoLogLength];
				int charactersWritten = 0;
				GL_CHECK(glGetShaderInfoLog(mId, infoLogLength, &charactersWritten, infoLog));
				// write somewhere!
				delete[] infoLog;
			}
			GL_CHECK(glDeleteShader(mId));
			mId = 0;
		}
	}
}

void ShaderGL::Destroy() {
	if (mId) {
		GL_CHECK(glDeleteShader(mId));
		mId = 0;
	}
}

} // namespace tinyngine