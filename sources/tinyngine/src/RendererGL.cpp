#include "RendererGL.h"

#include <vector>

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>

namespace tinygles
{

RendererGL::RendererGL() {

}

RendererGL::~RendererGL() {

}

void RendererGL::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	glViewport(x, y, width, height);
}

void RendererGL::Clear(ClearFlags flags, Color color, float depth, uint8_t stencil) {
	if (flags & ClearFlags::ColorBuffer) {
		glClearColor(color.red(), color.green(), color.blue(),color.alpha());
		glClear(GL_COLOR_BUFFER_BIT);
	}
	if (flags & ClearFlags::DepthBuffer) {
		glClearDepthf(depth);
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	if (flags & ClearFlags::StencilBuffer) {
		glClearStencil(stencil);
		glClear(GL_STENCIL_BUFFER_BIT);
	}
}

/*
static constexpr uint32_t cMaxShaderHandles = (1 << 6);
static constexpr uint32_t cMaxProgramHandles = (1 << 6);
static constexpr uint32_t cMaxVertexBufferHandles = (1 << 10);

struct ShaderGL {
	ShaderGL() : mId(0), mType(0) {}

	void Create(GLenum type, const char* source) {
		if (source != nullptr) {
			mId = glCreateShader(type);
			glShaderSource(mId, 1, (const char**)&source, nullptr);
			glCompileShader(mId);

			GLint compileResult;
			glGetShaderiv(mId, GL_COMPILE_STATUS, &compileResult);
			if (compileResult == 0) {
				GLint infoLogLength;
				glGetShaderiv(mId, GL_INFO_LOG_LENGTH, &infoLogLength);
				if (infoLogLength > 1) {
					char* infoLog = new char[infoLogLength];
					int charactersWritten = 0;
					glGetShaderInfoLog(mId, infoLogLength, &charactersWritten, infoLog);
					// write somewhere!
					delete[] infoLog;
				}
				glDeleteShader(mId);
				mId = 0;
			}
		}
	}

	void Destroy() {
		if (mId) {
			glDeleteShader(mId);
			mId = 0;
		}
	}

	GLuint mId;
	GLenum mType;
};

struct ProgramGL {
	ProgramGL() : mId(0) {}

	void Create(const ShaderGL& vs, const ShaderGL& fs) {
		mId = glCreateProgram();
		
		glAttachShader(mId, vs.mId);
		if (fs.mId) {
			glAttachShader(mId, fs.mId);
		}

		glLinkProgram(mId);

		GLint isLinked;
		glGetProgramiv(mId, GL_LINK_STATUS, &isLinked);
		if (!isLinked) {
			int infoLogLength;
			glGetProgramiv(mId, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 1) {
				int charactersWritten;
				char* infoLog = new char[infoLogLength];
				glGetProgramInfoLog(mId, infoLogLength, &charactersWritten, infoLog);
				// write somewhere
				delete[] infoLog;
			}
			glDeleteProgram(mId);
			mId = 0;
		}
	}

	GLuint mId;
};

struct VertexBufferGL {
	void Create(const void* data, uint32_t size) {
		glGenBuffers(1, &mId);
		glBindBuffer(GL_ARRAY_BUFFER, mId);
		glBufferData(GL_ARRAY_BUFFER, size, data, (data == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Update(const void* data, uint32_t offset, uint32_t size) {
		glBindBuffer(GL_ARRAY_BUFFER, mId);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Destroy() {
		if (mId) {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &mId);
			mId = 0;
		}
	}

	GLuint mId;
};

struct Clear {
	Color mColor;
	float mDepth;
	uint8_t mStencil;
	ClearFlags mFlags;
};

struct Renderer::Impl {
	Clear mClear;

	uint32_t mShadersCount;
	ShaderGL mShaders[cMaxShaderHandles];

	uint32_t mProgramsCount;
	ProgramGL mPrograms[cMaxProgramHandles];

	uint32_t mVertexBuffersCount;
	VertexBufferGL mVertexBuffers[cMaxVertexBufferHandles];

	VertexBufferHandle mCurrentVertexBufferHandle;
	ProgramHandle mCurrentProgramHandle;

	void Reset() {
		mShadersCount = 0;
		std::memset(mShaders, 0, sizeof(ShaderGL) * cMaxShaderHandles);

		mProgramsCount = 0;
		std::memset(mPrograms, 0, sizeof(ProgramGL) * cMaxProgramHandles);

		mVertexBuffersCount = 0;
		std::memset(mVertexBuffers, 0, sizeof(VertexBufferGL) * cMaxVertexBufferHandles);
	}

	void Release() {
		for (auto& vb : mVertexBuffers) {
			vb.Destroy();
		}
	}
};

//====================================================================================================================

Renderer* Renderer::GetRenderer() {
	static Renderer* sRenderer = nullptr;
	if (!sRenderer) {
		sRenderer = new Renderer();
	}
	return sRenderer;
}

Renderer::Renderer() : mImpl(std::make_unique<Impl>()) {
	mImpl->Reset();
}

Renderer::~Renderer() {
	mImpl->Release();
}

Renderer::Renderer(Renderer& rhs) : mImpl(nullptr) {
	if (rhs.mImpl != nullptr) {
		mImpl = std::make_unique<Impl>((*rhs.mImpl));
	}
}

Renderer& Renderer::operator=(Renderer& rhs) {
	if (rhs.mImpl == nullptr) {
		mImpl.reset();
	}
	else if (mImpl == nullptr) {
		mImpl = std::make_unique<Impl>((*rhs.mImpl));
	}
	else {
		*mImpl = *rhs.mImpl;
	}
	return (*this);
}

//====================================================================================================================
//====================================================================================================================

void Renderer::BeginFrame() {
	if ((mImpl->mClear.mFlags & ClearFlags::Color) == ClearFlags::Color) {
		glClearColor(mImpl->mClear.mColor.red(), mImpl->mClear.mColor.green(), mImpl->mClear.mColor.blue(), mImpl->mClear.mColor.alpha());
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void Renderer::EndFrame() {
	glFinish();
}

void Renderer::SetViewClear(ClearFlags flags, Color rgba, float depth, uint8_t stencil) {
	mImpl->mClear.mFlags = flags;
	mImpl->mClear.mColor = rgba;
	mImpl->mClear.mDepth = depth;
	mImpl->mClear.mStencil = stencil;
}

void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	glViewport(x, y, width, height);
}

ShaderHandle Renderer::CreateShader(uint32_t type, const char* source) {
	ShaderHandle handle = ShaderHandle(mImpl->mShadersCount);
	mImpl->mShaders[handle.mHandle].Create(type, source);
	mImpl->mShadersCount++;
	return handle;
}

ProgramHandle Renderer::CreateProgram(const ShaderHandle& vsh, const ShaderHandle& fsh, bool destroyShaders) {
	if (!vsh.IsValid()) {
		return ProgramHandle(0);
	}

	ProgramHandle handle = ProgramHandle(mImpl->mProgramsCount);

	auto& vs = mImpl->mShaders[vsh.mHandle];
	auto& fs = mImpl->mShaders[fsh.mHandle];
	mImpl->mPrograms[handle.mHandle].Create(vs, fs);
	mImpl->mShadersCount++;

	if (destroyShaders) {
		vs.Destroy();
		fs.Destroy();
	}

	return handle;
}

void Renderer::SetProgramUniform(const ProgramHandle& handle) {
	TINYGLES_UNUSED(handle);

}

VertexBufferHandle Renderer::CreateVertexBuffer(const void* data, uint32_t size) {
	VertexBufferHandle handle = VertexBufferHandle(mImpl->mVertexBuffersCount);
	mImpl->mVertexBuffers[handle.mHandle].Create(data, size);
	mImpl->mVertexBuffersCount++;
	return handle;
}

void Renderer::SetProgram(const ProgramHandle& handle) {
	mImpl->mCurrentProgramHandle = handle;
	auto& progrm = mImpl->mPrograms[mImpl->mCurrentProgramHandle.mHandle];
	glUseProgram(progrm.mId);
}

void Renderer::SetVertexBuffer(const VertexBufferHandle& handle) {
	mImpl->mCurrentVertexBufferHandle = handle;
}

void Renderer::DrawArray(uint32_t first, uint32_t count) {
	//auto& progrm = mImpl->mPrograms[mImpl->mCurrentProgramHandle.mHandle];
	auto& vb = mImpl->mVertexBuffers[mImpl->mCurrentVertexBufferHandle.mHandle];

	//glUseProgram(progrm.mId);

	glBindBuffer(GL_ARRAY_BUFFER, vb.mId);
	glDrawArrays(GL_TRIANGLES, first, count);
}
*/

}