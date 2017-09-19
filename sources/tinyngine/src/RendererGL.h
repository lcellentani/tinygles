#pragma once

#include "Renderer.h"

namespace tinygles
{

class RendererGL : public Renderer {
public:
	RendererGL();
	virtual ~RendererGL();

	virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

	virtual void Clear(ClearFlags flags, Color color, float depth = 1.0f, uint8_t stencil = 0) override;
};

/*
enum class ClearFlags : uint8_t {
	None = 0,
	Color = 1 << 0,
	Depth = 1 << 1,
	Stencil = 1 << 2
};

inline constexpr ClearFlags operator&(ClearFlags x, ClearFlags y) {
	return static_cast<ClearFlags>(static_cast<int>(x) & static_cast<int>(y));
}

inline constexpr ClearFlags operator|(ClearFlags x, ClearFlags y) {
	return static_cast<ClearFlags>(static_cast<int>(x) | static_cast<int>(y));
}

inline constexpr ClearFlags operator^(ClearFlags x, ClearFlags y) {
	return static_cast<ClearFlags>(static_cast<int>(x) ^ static_cast<int>(y));
}

inline constexpr ClearFlags operator~(ClearFlags x) {
	return static_cast<ClearFlags>(~static_cast<int>(x));
}

inline ClearFlags& operator&=(ClearFlags & x, ClearFlags y) {
	x = x & y;
	return x;
}

inline ClearFlags& operator|=(ClearFlags & x, ClearFlags y) {
	x = x | y;
	return x;
}

inline ClearFlags& operator^=(ClearFlags & x, ClearFlags y) {
	x = x ^ y;
	return x;
}

struct ResourceHandle {
	ResourceHandle() {};
	ResourceHandle(uint32_t handle) : mHandle(handle) {}
	inline const bool IsValid() const { return mHandle != 0; }
	uint32_t mHandle = 0;
};

using ShaderHandle = ResourceHandle;
using ProgramHandle = ResourceHandle;
using VertexBufferHandle = ResourceHandle;

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
} // tinygles