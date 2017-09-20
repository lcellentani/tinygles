#include "ShaderGL.h"

namespace tinyngine
{

void ShaderGL::Create(GLenum type, const char* source) {
	if (source != nullptr) {
		mId = glCreateShader(type);
		glShaderSource(mId, 1, (const char**)&source, nullptr);
		glCompileShader(mId);

		GLint compileResult;
		glGetShaderiv(mId, GL_COMPILE_STATUS, &compileResult);
		if (compileResult == 0) {
			GLint infoLogLength;
			glGetShaderiv(mId, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 1) {
				char* infoLog = new char[infoLogLength];
				int charactersWritten = 0;
				glGetShaderInfoLog(mId, infoLogLength, &charactersWritten, infoLog);
				// write somewhere!
				delete[] infoLog;
			}
			glDeleteShader(mId);
			mId = 0;
		}
	}
}

void ShaderGL::Destroy() {
	if (mId) {
		glDeleteShader(mId);
		mId = 0;
	}
}

} // namespace tinyngine