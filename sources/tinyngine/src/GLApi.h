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

GLenum GetShaderType(ShaderType::Enum type);

GLenum GetPrimitiveType(PrimitiveType::Enum type);

GLenum GetRendererStateType(RendererStateType::Enum type);
GLenum GetBlendFunc(BlendFuncs::Enum func);
GLenum GetCullFaceMode(CullFaceModes::Enum mode);
GLenum GetWindingMode(WindingModes::Enum mode);
GLenum GetDepthFunc(DepthFuncs::Enum func);
GLenum GetStencilFunc(StencilFuncs::Enum func);
GLenum GetStencilOpType(StencilOpTypes::Enum type);

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
