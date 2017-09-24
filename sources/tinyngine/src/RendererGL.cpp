#include "RendererGL.h"
#include "PlatformDefine.h"
#include "ShaderGL.h"
#include "ProgramGL.h"
#include "VertexBufferGL.h"

#include <array>
#include <unordered_map>

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>

namespace
{

static constexpr uint32_t cMaxShaderHandles = (1 << 6);
static constexpr uint32_t cMaxProgramHandles = (1 << 6);
static constexpr uint32_t cMaxVertexBufferHandles = (1 << 10);

}

namespace tinyngine
{

struct RendererGL::Impl {
	uint32_t mVertexBuffersCount = 0;
	std::array<VertexBufferGL, cMaxVertexBufferHandles> mVertexBuffers;
	VertexBufferHandle mCurrentVertexBufferHandle = ResourceHandle(cInvalidHandle);

	uint32_t mShadersCount = 0;
	std::array<ShaderGL, cMaxShaderHandles> mShaders;

	uint32_t mProgramsCount = 0;
	std::array<ProgramGL, cMaxProgramHandles> mPrograms;
	ProgramHandle mCurrentProgramHandle = ResourceHandle(cInvalidHandle);
	ProgramGL mInvalidProgram;
};

//=====================================================================================================================

RendererGL::RendererGL() : mImpl(new Impl()) {
}

RendererGL::~RendererGL() {

}

void RendererGL::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	GL_CHECK(glViewport(x, y, width, height));
}

void RendererGL::Clear(ClearFlags flags, Color color, float depth, uint8_t stencil) {
	if (flags & ClearFlags::ColorBuffer) {
		GL_CHECK(glClearColor(color.red(), color.green(), color.blue(), color.alpha()));
		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
	}
	if (flags & ClearFlags::DepthBuffer) {
		GL_CHECK(glClearDepthf(depth));
		GL_CHECK(glClear(GL_DEPTH_BUFFER_BIT));
	}
	if (flags & ClearFlags::StencilBuffer) {
		GL_CHECK(glClearStencil(stencil));
		GL_CHECK(glClear(GL_STENCIL_BUFFER_BIT));
	}
}

void RendererGL::Commit() {
	GL_CHECK(glFinish());
	if (mImpl->mCurrentProgramHandle.IsValid()) {
		auto& program = mImpl->mPrograms[mImpl->mCurrentProgramHandle.mHandle];
		program.UnbindAttributes();
		mImpl->mCurrentProgramHandle = ProgramHandle(cInvalidHandle);
	}
	if (mImpl->mCurrentVertexBufferHandle.IsValid()) {
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
		mImpl->mCurrentVertexBufferHandle = VertexBufferHandle(cInvalidHandle);
	}
}

void RendererGL::DrawArray(PrimitiveType::Enum primitive, uint32_t first, uint32_t count) {
	GL_CHECK(glDrawArrays(tinyngine::gl::GetPrimitiveType(primitive), first, count));
}

VertexBufferHandle RendererGL::CreateVertexBuffer(const void* data, uint32_t size, const VertexFormat& vertexFormat) {
	VertexBufferHandle handle = VertexBufferHandle(mImpl->mVertexBuffersCount++);
	auto& vertexBuffer = mImpl->mVertexBuffers[handle.mHandle];
	vertexBuffer.Create(data, size, vertexFormat);
	return vertexBuffer.IsValid() ? handle : VertexBufferHandle(cInvalidHandle);
}

void RendererGL::SetVertexBuffer(VertexBufferHandle& handle) {
	if (handle.IsValid()) {
		mImpl->mCurrentVertexBufferHandle = handle;
		auto& vertexBuffer = mImpl->mVertexBuffers[handle.mHandle];
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.GetId()));
	}
}

ShaderHandle RendererGL::CreateShader(ShaderType::Enum type, const char* source) {
	ShaderHandle handle = ShaderHandle(mImpl->mShadersCount++);
	auto& shader = mImpl->mShaders[handle.mHandle];
	shader.Create(tinyngine::gl::GetShaderType(type), source);
	return shader.IsValid() ? handle : ShaderHandle(cInvalidHandle);
}

ProgramHandle RendererGL::CreateProgram(ShaderHandle& vertexShaderHandle, ShaderHandle& fragmentShaderHandle, bool destroyShaders) {
	if (!vertexShaderHandle.IsValid()) {
		return ProgramHandle(cInvalidHandle);
	}

	auto& vertexShader = mImpl->mShaders[vertexShaderHandle.mHandle];
	auto& fragmentShader = mImpl->mShaders[fragmentShaderHandle.mHandle];

	ProgramHandle handle = ProgramHandle(mImpl->mProgramsCount++);
	auto& program = mImpl->mPrograms[handle.mHandle];
	program.Create(vertexShader, fragmentShader);

	program.Initialize();

	if (destroyShaders) {
		vertexShader.Destroy();
		fragmentShader.Destroy();
	}

	return program.IsValid() ? handle : ProgramHandle(cInvalidHandle);
}

void RendererGL::SetProgram(ProgramHandle handle, const VertexFormat& vertexFormat) {
	if (mImpl->mCurrentProgramHandle.IsValid()) {
		auto& program = mImpl->mPrograms[mImpl->mCurrentProgramHandle.mHandle];
		program.UnbindAttributes();
	}

	mImpl->mCurrentProgramHandle = handle;
	if (mImpl->mCurrentProgramHandle.IsValid()) {
		auto& program = mImpl->mPrograms[mImpl->mCurrentProgramHandle.mHandle];
		GL_CHECK(glUseProgram(program.GetId()));
		program.BindAttributes(vertexFormat, 0);
	}
}

void RendererGL::SetUniformMat4(ProgramHandle handle, Uniforms::Enum uniformName, float* data, bool transpose) {
	if (!handle.IsValid()) {
		return;
	}
	auto& program = mImpl->mPrograms[handle.mHandle];
	auto& uniform = program.GetUniform(uniformName);
	GL_CHECK(glUniformMatrix4fv(uniform.mLocation, uniform.mSize, transpose ? GL_TRUE : GL_FALSE, data));
}

} // namespace tinyngine