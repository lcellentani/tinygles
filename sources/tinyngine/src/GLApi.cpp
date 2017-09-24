#include "GLApi.h"

namespace {

static const GLenum cAttributeTypes[]{
	GL_UNSIGNED_BYTE,
	GL_SHORT,
	GL_FLOAT
};

static const char* cAttributeNames[] =
{
	"a_position",
	"a_normal",
	"a_tangent",
	"a_bitangent",
	"a_color0",
	"a_color1",
	"a_texcoord0",
	"a_texcoord1",
	"a_texcoord2",
	"a_texcoord3"
};

static const char* cPredefinedUniformName[] =
{
	"u_view",
	"u_proj",
	"u_model",
	"u_viewProj",
	"u_modelView",
	"u_modelViewProj"
};

static const GLenum cShaderType[]{
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER
};

static const GLenum cPrimitiveTypes[] = {
	GL_TRIANGLES,
	GL_TRIANGLE_STRIP,
	GL_LINES,
	GL_LINE_STRIP,
	GL_POINTS
};

}

namespace tinyngine {
namespace gl {

const char* GetErrorString(GLenum _enum) {
#define GLENUM(_ty) case _ty: return #_ty
	switch (_enum) {
		GLENUM(GL_TEXTURE);
		GLENUM(GL_RENDERBUFFER);

		GLENUM(GL_INVALID_ENUM);
		GLENUM(GL_INVALID_FRAMEBUFFER_OPERATION);
		GLENUM(GL_INVALID_VALUE);
		GLENUM(GL_INVALID_OPERATION);
		GLENUM(GL_OUT_OF_MEMORY);

		GLENUM(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
		GLENUM(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);

		GLENUM(GL_FRAMEBUFFER_UNSUPPORTED);
	}
	return "<unknown>";
}

GLenum GetAttributeType(AttributeType::Enum type) {
	return cAttributeTypes[type];
}

const char* GetAttributeName(Attributes::Enum attribute) {
	return cAttributeNames[attribute];
}

const char* GetPredefinedUniformName(Uniforms::Enum uniform) {
	return cPredefinedUniformName[uniform];
}

GLenum GetShaderType(ShaderType::Enum type) {
	return cShaderType[type];
}

GLenum GetPrimitiveType(PrimitiveType::Enum type) {
	return cPrimitiveTypes[type];
}

}
}