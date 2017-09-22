#pragma once

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
#include <GLES2/gl2.h>

#include "Log.h"
extern const char* glEnumName(GLenum _enum);

#define GL_CHECK(_call) \
			for(;;) { \
			_call; \
			GLenum glError = glGetError(); \
			if (glError != GL_NO_ERROR) { Log(tinyngine::Logger::Error, "GL error 0x%x %s", glError, tinyngine::gl::glEnumName(glError)); abort(); } \
			break; }
