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

	virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

	virtual void Commit() = 0;

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

	virtual void SetUniformMat4(const ProgramHandle& handle, Uniforms::Enum uniform, const float* data, bool transpose) = 0;
};

/*
class Renderer {
public:
	static Renderer* GetRenderer();

public:
	Renderer();
	~Renderer();

	Renderer(Renderer& rhs);
	Renderer& operator=(Renderer& rhs);

	Renderer(Renderer&& rhs) noexcept = default;
	Renderer& operator= (Renderer&& rhs) noexcept = default;

public:
	void BeginFrame();
	void EndFrame();

	void SetViewClear(ClearFlags flags, Color rgba, float depth = 1.0f, uint8_t stencil = 0);
	void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

	ShaderHandle CreateShader(uint32_t type, const char* source);
	ProgramHandle CreateProgram(const ShaderHandle& vsh, const ShaderHandle& fsh, bool destroyShaders);
	
	void SetProgramUniform(const ProgramHandle& handle);

	VertexBufferHandle CreateVertexBuffer(const void* data, uint32_t size);

	void SetProgram(const ProgramHandle& handle);
	void SetVertexBuffer(const VertexBufferHandle& handle);

	void DrawArray(uint32_t first, uint32_t count);

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};
*/
} // namespace tinyngine