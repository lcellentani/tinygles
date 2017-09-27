#pragma once

//#ifndef GL_GLEXT_PROTOTYPES
//#define GL_GLEXT_PROTOTYPES
//#endif
//#include <GLES2/gl2.h>
#include <GLES3/gl3.h>

#include <cstdlib>

#include "GraphicsTypes.h"
#include "Log.h"

namespace tinyngine {
namespace gl {

const char* GetErrorString(GLenum _enum);

GLenum GetAttributeType(AttributeType::Enum type);
const char* GetAttributeName(Attributes::Enum attribute);

const char* GetPredefinedUniformName(Uniforms::Enum uniform);

GLenum GetShaderType(ShaderType::Enum type);

GLenum GetPrimitiveType(PrimitiveType::Enum type);

} // namespace gl
} // namespace tinyngine

#define GL_ERROR(condition) \
			if (condition) { GLenum glError = glGetError(); if (glError != GL_NO_ERROR) { Log(tinyngine::Logger::Error, "GL error 0x%x %s", glError, tinyngine::gl::GetErrorString(glError)); abort(); } }

#define GL_CHECK(_call) \
			for(;;) { \
			_call; \
			GLenum glError = glGetError(); \
			if (glError != GL_NO_ERROR) { Log(tinyngine::Logger::Error, "GL error 0x%x %s", glError, tinyngine::gl::GetErrorString(glError)); abort(); } \
			break; }
