#include "ShadersUtil.h"

GLuint CompileShader(GLenum type, const char * source, std::string& errorLog) {
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, (const char**)&source, nullptr);
	glCompileShader(shader);

	GLint compileResult;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

	if (compileResult == 0) {
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 1) {
			char* infoLog = new char[infoLogLength];
			int charactersWritten = 0;
			glGetShaderInfoLog(shader, infoLogLength, &charactersWritten, infoLog);
			errorLog = infoLog;
			delete[] infoLog;
		}
		glDeleteShader(shader);
		shader = 0;
	}

	return shader;
}

GLuint CompileProgram(const char * vsSource, const char * fsSource) {
	TINYGLES_UNUSED(vsSource); TINYGLES_UNUSED(fsSource);
	return 0;
}
