#include "GLApi.h"

namespace {

static const GLenum cAttributeTypes[]{
	GL_UNSIGNED_BYTE,
	GL_SHORT,
	GL_FLOAT
};

static const char* cAttributeNames[]{
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

static const GLenum cShaderType[]{
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER
};

static const GLenum cPrimitiveTypes[]{
	GL_TRIANGLES,
	GL_TRIANGLE_STRIP,
	GL_LINES,
	GL_LINE_STRIP,
	GL_POINTS
};

static const GLenum cStateTypes[]{
	GL_BLEND,
	GL_CULL_FACE,
	GL_DEPTH_TEST,
	GL_POLYGON_OFFSET_FILL,
	GL_SCISSOR_TEST,
	GL_STENCIL_TEST
};

static const GLenum cBlendFuncs[]{
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_CONSTANT_COLOR,
	GL_ONE_MINUS_CONSTANT_COLOR,
	GL_CONSTANT_ALPHA,
	GL_ONE_MINUS_CONSTANT_ALPHA,
	GL_SRC_ALPHA_SATURATE
};

static const GLenum cCullFaceModes[]{
	GL_FRONT,
	GL_BACK,
	GL_FRONT_AND_BACK
};

static const GLenum cWindingMode[]{
	GL_CW,
	GL_CCW
};

static const GLenum cDepthFuncs[]{
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS
};

static const GLenum cStencilFuncs[]{
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS
};

static const GLenum cStencilOpTypes[]{
	GL_KEEP,
	GL_ZERO,
	GL_REPLACE,
	GL_INCR,
	GL_INCR_WRAP,
	GL_DECR,
	GL_DECR_WRAP,
	GL_INVERT
};

static const GLint cWrapMode[]{
	GL_REPEAT,
	GL_CLAMP_TO_EDGE,
	GL_MIRRORED_REPEAT,
	0 //OpenGL specific GL_CLAMP_TO_BORDER
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

GLenum GetShaderType(ShaderType::Enum type) {
	return cShaderType[type];
}

GLenum GetPrimitiveType(PrimitiveType::Enum type) {
	return cPrimitiveTypes[type];
}

GLenum GetRendererStateType(RendererStateType::Enum type) {
	return cStateTypes[type];
}

GLenum GetBlendFunc(BlendFuncs::Enum func) {
	return cBlendFuncs[func];
}

GLenum GetCullFaceMode(CullFaceModes::Enum mode) {
	return cCullFaceModes[mode];
}

GLenum GetWindingMode(WindingModes::Enum mode) {
	return cWindingMode[mode];
}

GLenum GetDepthFunc(DepthFuncs::Enum func) {
	return cDepthFuncs[func];
}

GLenum GetStencilFunc(StencilFuncs::Enum func) {
	return cStencilFuncs[func];
}

GLenum GetStencilOpType(StencilOpTypes::Enum type) {
	return cStencilOpTypes[type];
}

GLint GetWrapMode(TextureWrapMode::Enum mode) {
	return cWrapMode[mode];
}

}
}