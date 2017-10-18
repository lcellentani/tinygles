#pragma once

#include "GraphicsTypes.h"
#include "VertexFormat.h"

namespace tinyngine
{

class Renderer {
public:
	enum ClearFlags {
		None = 0,
		ColorBuffer = 1 << 0,
		DepthBuffer = 1 << 1,
		StencilBuffer = 1 << 2
	};

public:
	virtual ~Renderer() = default;

	virtual void Commit() = 0;

	virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

	virtual void Clear(uint8_t flags, Color color, float depth = 1.0f, uint8_t stencil = 0) = 0;
	virtual void SetColorMake(bool red, bool green, bool blue, bool alpha) = 0;
	virtual void SetDepthMask(bool flag) = 0;
	virtual void SetStencilMask(uint32_t mask) = 0;

	virtual void SetState(RendererStateType::Enum type, bool value) = 0;
	virtual void SetCullMode(CullFaceModes::Enum mode) = 0;
	virtual void SetWinding(WindingModes::Enum mode) = 0;
	virtual void SetBlendFunc(BlendFuncs::Enum sfactor, BlendFuncs::Enum dfactor) = 0;
	virtual void SetDepthFunc(DepthFuncs::Enum func) = 0;
	virtual void SetStencilFunc(StencilFuncs::Enum func, int32_t ref, uint32_t mask) = 0;
	virtual void SetStencilOp(StencilOpTypes::Enum sfail, StencilOpTypes::Enum dpfail, StencilOpTypes::Enum dppass) = 0;
	virtual void SetBlendColor(Color color) = 0;
	virtual void SetPolygonffset(float factor, float units) = 0;

	virtual void DrawArray(PrimitiveType::Enum primitive, uint32_t first, uint32_t count) = 0;
	virtual void DrawElements(PrimitiveType::Enum primitive, uint32_t count) = 0;

	virtual VertexBufferHandle CreateVertexBuffer(const void* data, uint32_t size, const VertexFormat& vertexFormat) = 0;
	virtual void SetVertexBuffer(const VertexBufferHandle& handle, Attributes::Enum attribute) = 0;

	virtual IndexBufferHandle CreateIndexBuffer(const void* data, uint32_t size) = 0;
	virtual void SetIndexBuffer(const IndexBufferHandle& handle) = 0;

	virtual ShaderHandle CreateShader(ShaderType::Enum tpye, const char* source) = 0;

	virtual ProgramHandle CreateProgram(ShaderHandle& vertexShaderHandle, ShaderHandle& fragmentShaderHandle, bool destroyShaders) = 0;
	virtual void SetProgram(const ProgramHandle& handle, const VertexFormat& vertexFormat) = 0;

	virtual UniformHandle GetUniform(const ProgramHandle& handle, const char* uniformName) const = 0;
	virtual void SetUniformFloat(const ProgramHandle& programHandle, UniformHandle& uniformHandle, float data) = 0;
	virtual void SetUniformFloat3(const ProgramHandle& programHandle, UniformHandle& uniformHandle, const float* data) = 0;
	virtual void SetUniformMat4(const ProgramHandle& programHandle, const UniformHandle& uniformHandle, const float* data, bool transpose) = 0;
};

} // namespace tinyngine