#pragma once

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
#include <GLES2/gl2.h>
#include <cstdlib>

#include "GraphicsTypes.h"
#include "Log.h"

namespace tinyngine {
namespace gl {

const char* GetEnumName(GLenum _enum);

GLenum GetAttributeType(AttributeType::Enum type);
const char* GetAttributeName(Attributes::Enum attribute);

const char* GetPredefinedUniformName(Uniforms::Enum uniform);


} // namespace gl
} // namespace tinyngine

#define GL_ERROR(condition) \
			if (condition) { GLenum glError = glGetError(); if (glError != GL_NO_ERROR) { Log(tinyngine::Logger::Error, "GL error 0x%x %s", glError, tinyngine::gl::GetEnumName(glError)); abort(); } }

#define GL_CHECK(_call) \
			for(;;) { \
			_call; \
			GLenum glError = glGetError(); \
			if (glError != GL_NO_ERROR) { Log(tinyngine::Logger::Error, "GL error 0x%x %s", glError, tinyngine::gl::GetEnumName(glError)); abort(); } \
			break; }
