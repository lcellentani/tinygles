#include "ShadersUtil.h"
#include <iostream>
#include <fstream>

namespace
{

static bool ReadFileToString(const char* source, std::string& content) {
	bool result = false;
	if (source) {
		std::ifstream stream(source);
		if (stream) {
			stream.seekg(0, std::ios::end);
			content.reserve(static_cast<unsigned int>(stream.tellg()));
			stream.seekg(0, std::ios::beg);

			content.assign((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

			result = true;
		}
	}
	return result;
}

}

namespace tinygles
{

GLuint CompileShader(GLenum type, const char * source, std::function<void(GLenum type, const char*)> func) {
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, (const char**)&source, nullptr);
	glCompileShader(shader);

	GLint compileResult;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

	if (compileResult == 0) {
		if (func) {
			GLint infoLogLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 1) {
				char* infoLog = new char[infoLogLength];
				int charactersWritten = 0;
				glGetShaderInfoLog(shader, infoLogLength, &charactersWritten, infoLog);
				func(type, infoLog);
				delete[] infoLog;
			}
		}
		glDeleteShader(shader);
		shader = 0;
	}

	return shader;
}

GLuint CompileShaderFromFile(GLenum type, const char* sourcePath, std::function<void(GLenum type, const char*)> func) {
	std::string source;
	if (!::ReadFileToString(sourcePath, source)) {
		return 0;
	}
	if (source.empty()) {
		return 0;
	}

	return CompileShader(type, source.c_str(), func);
}

GLuint CompileProgram(const char * vsSource, const char * fsSource, std::function<void(GLenum type, const char*)> func) {
	GLuint program = glCreateProgram();

	GLuint vs = CompileShader(GL_VERTEX_SHADER, vsSource, func);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fsSource, func);

	if (vs == 0 || fs == 0) {
		glDeleteShader(fs);
		glDeleteShader(vs);
		glDeleteProgram(program);
		return 0;
	}

	glAttachShader(program, vs);
	glDeleteShader(vs);

	glAttachShader(program, fs);
	glDeleteShader(fs);

	glLinkProgram(program);
	GLint isLinked;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (!isLinked) {
		if (func) {
			// If an error happened, first retrieve the length of the log message
			int infoLogLength, charactersWritten;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

			// Allocate enough space for the message and retrieve it
			char* infoLog = new char[infoLogLength];
			glGetProgramInfoLog(program, infoLogLength, &charactersWritten, infoLog);
			func(0, infoLog);
			delete[] infoLog;
		}
		glDeleteProgram(program);
		program = 0;
	}

	return program;
}

GLuint CompileProgramFromFile(const char * vsSourcePath, const char * fsSourcePath, std::function<void(GLenum type, const char*)> func) {
	std::string vsSource;
	if (!ReadFileToString(vsSourcePath, vsSource)) {
		return 0;
	}
	std::string fsSource;
	if (!ReadFileToString(fsSourcePath, fsSource)) {
		return 0;
	}
	if (vsSource.empty() || fsSource.empty()) {
		return 0;
	}

	return CompileProgram(vsSource.c_str(), fsSource.c_str(), func);
}

}