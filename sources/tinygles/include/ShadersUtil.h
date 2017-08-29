#pragma once

#include "PlatformDefine.h"
#include <string>

#define SHADER_SOURCE(...) #__VA_ARGS__

GLuint CompileShader(GLenum type, const char * source, std::string& errorLog);
GLuint CompileProgram(const char * vsSource, const char * fsSource);