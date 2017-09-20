#pragma once

#include "PlatformDefine.h"
#include <functional>

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
#include <GLES2/gl2.h>

#define SHADER_SOURCE(...) #__VA_ARGS__

namespace tinyngine
{

GLuint CompileShader(GLenum type, const char * source, std::function<void(GLenum type, const char*)> func);
GLuint CompileShaderFromFile(GLenum type, const char* sourcePath, std::function<void(GLenum type, const char*)> func);

GLuint CompileProgram(const char * vsSource, const char * fsSource, std::function<void(GLenum type, const char*)> func);
GLuint CompileProgramFromFile(const char * vsSourcePath, const char * fsSourcePath, std::function<void(GLenum type, const char*)> func);

} // namespace tinyngine