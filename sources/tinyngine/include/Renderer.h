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

	virtual void Clear(ClearFlags flags, Color color, float depth = 1.0f, uint8_t stencil = 0) = 0;
	virtual void Commit() = 0;

	virtual ShaderHandle CreateShader(ShaderType tpye, const char* source) = 0;

	virtual ProgramHandle CreateProgram(ShaderHandle& vsh, ShaderHandle& fsh, bool destroyShaders) = 0;
	virtual void SetProgram(ProgramHandle program, const VertexFormat& vertexFormat) = 0;

	virtual void SetUniformMat4(ProgramHandle program, Uniforms::Enum uniform, float* data, bool transpose) = 0;
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