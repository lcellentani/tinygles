#include "Renderer.h"

#include <vector>

namespace tinygles
{

struct Clear {
	Color mColor;
	float mDepth;
	uint8_t mStencil;
	ClearFlags mFlags;
};

struct Renderer::Impl {
	Clear mClear;
	ShaderHandle mShaderHandles;
	ProgramHandle mProgramHandles;
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

}

Renderer::~Renderer() = default;

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

ShaderHandle Renderer::CreateShader(uint32_t type, const char* source, std::function<void(uint32_t, const char*)> func) {
	GLuint shaderId = 0;
	if (source != nullptr) {
		shaderId = glCreateShader(type);
		if (shaderId) {
			glShaderSource(shaderId, 1, (const char**)&source, nullptr);
			glCompileShader(shaderId);

			GLint compileResult;
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileResult);

			if (compileResult == 0) {
				if (func) {
					GLint infoLogLength;
					glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
					if (infoLogLength > 1) {
						char* infoLog = new char[infoLogLength];
						int charactersWritten = 0;
						glGetShaderInfoLog(shaderId, infoLogLength, &charactersWritten, infoLog);
						func(type, infoLog);
						delete[] infoLog;
					}
				}
				glDeleteShader(shaderId);
				shaderId = 0;
			}
		}
	}
	return ShaderHandle(shaderId);
}

ProgramHandle Renderer::CreateProgram(const char* vsSource, const char* fsSource, std::function<void(uint32_t, const char*)> func) {
	ShaderHandle vsHandle = CreateShader(GL_VERTEX_SHADER, vsSource, func);
	ShaderHandle fsHandle = CreateShader(GL_FRAGMENT_SHADER, fsSource, func);

	if (!vsHandle.IsValid() || !fsHandle.IsValid()) {
		glDeleteShader(vsHandle.mHandle);
		glDeleteShader(fsHandle.mHandle);
		return ProgramHandle(0);
	}

	GLuint programId = glCreateProgram();

	glAttachShader(programId, vsHandle.mHandle);
	glDeleteShader(vsHandle.mHandle);

	glAttachShader(programId, fsHandle.mHandle);
	glDeleteShader(fsHandle.mHandle);

	glLinkProgram(programId);
	GLint isLinked;
	glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);
	if (!isLinked) {
		if (func) {
			// If an error happened, first retrieve the length of the log message
			int infoLogLength, charactersWritten;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);

			// Allocate enough space for the message and retrieve it
			char* infoLog = new char[infoLogLength];
			glGetProgramInfoLog(programId, infoLogLength, &charactersWritten, infoLog);
			func(0, infoLog);
			delete[] infoLog;
		}
		glDeleteProgram(programId);
		programId = 0;
	}
	return ProgramHandle(programId);
}

}