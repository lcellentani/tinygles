#include "Application.h"
#include "GeometryUtil.h"
#include "ShadersUtil.h"
#include "Log.h"

using namespace tinygles;

class SimpleMinmapping : public Application {
public:
	SimpleMinmapping() = default;
	virtual ~SimpleMinmapping() {

	}

	ContextAttribs& GetContextAttribs() override {
		static ContextAttribs sAttributes;

		sAttributes.mRequiredApi = Api::OpenGLES2;
		sAttributes.mDepthBPP = 32;
		sAttributes.mStencilBPP = 0;
		sAttributes.mRedBits = 8;
		sAttributes.mGreenBits = 8;
		sAttributes.mBlueBits = 8;
		sAttributes.mAlphaBits = 8;

		return sAttributes;
	}

	void InitApplication() override {

	}

	void InitView(std::unique_ptr<Renderer>& renderer) override {
		TINYGLES_UNUSED(renderer);

		InitializeShaders();

		CreateTexture(256, 256);
	}

	void RenderFrame(std::unique_ptr<Renderer>& renderer) override {
		TINYGLES_UNUSED(renderer);

		const GLfloat vertices[]{
			-0.5f, 0.5f, 0.0f, 2.0f,
			0.0f, 0.0f,
			-0.5f, -0.5f, 0.0f, 1.25f,
			0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.25f,
			1.0f, 1.0f,
			0.5f, 0.5f, 0.0f, 2.0f,
			1.0f, 0.0f
		};
		const GLushort indices[]{
			0, 1, 2, 0, 2, 3
		};

		GLenum lastError;

		glClearColor(0.36f, 0.36f, 0.36f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(mShaderProgram);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glVertexAttribPointer(mPositionAttributePos, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const void*)&vertices[0]);
		glEnableVertexAttribArray(mPositionAttributePos);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glVertexAttribPointer(mTexcoordAttributePos, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const void*)&vertices[4]);
		glEnableVertexAttribArray(mTexcoordAttributePos);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTextureId);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glUniform1i(mSamplerPos, 0);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glUniform1f(mOffsetPos, -0.6f);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void*)&indices[0]);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glUniform1f(mOffsetPos, 0.6f);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void*)&indices[0]);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glFinish();
	}

	void ReleaseView(std::unique_ptr<Renderer>& renderer) override {
		TINYGLES_UNUSED(renderer);
		glDeleteProgram(mShaderProgram);
		glDeleteTextures(1, &mTextureId);
	}

	void ReleaseApplication() override {

	}

	void OnReshape(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override {
		TINYGLES_UNUSED(x); TINYGLES_UNUSED(y);
		mWindowWidth = width;
		mWindowHeight = height;
		mAspect = (float)mWindowWidth / (float)mWindowHeight;
	}

private:
	bool GenMipmap2D(const std::vector<GLubyte>& src, std::vector<GLubyte>& dst, const uint32_t srcWidth, const uint32_t srcHeight, uint32_t& dstWidth, uint32_t& dstHeight) {
		constexpr int32_t texelSize = 3;

		dstWidth = srcWidth / 2;
		if (dstWidth == 0) {
			dstWidth = 1;
		}
		dstHeight = srcHeight / 2;
		if (dstHeight == 0) {
			dstHeight = 1;
		}

		dst.resize(texelSize * dstWidth * dstHeight);
		for (uint32_t y = 0; y < dstHeight; y++) {
			for (uint32_t x = 0; x < dstWidth; x++) {
				uint32_t srcIndexes[4]{ 0 };
				float color[3]{ 0 };

				srcIndexes[0] = (((y * 2) * srcWidth) + (x * 2)) * texelSize;
				srcIndexes[1] = (((y * 2) * srcWidth) + (x * 2 + 1)) * texelSize;
				srcIndexes[2] = ((((y * 2) + 1) * srcWidth) + (x * 2)) * texelSize;
				srcIndexes[3] = ((((y * 2) + 1) * srcWidth) + (x * 2 + 1)) * texelSize;

				for (uint32_t sample = 0; sample < 4; sample++) {
					color[0] += src[srcIndexes[sample]];
					color[1] += src[srcIndexes[sample] + 1];
					color[2] += src[srcIndexes[sample] + 2];
				}

				color[0] /= 4.0;
				color[1] /= 4.0;
				color[2] /= 4.0;

				dst[(y * dstWidth + x) * texelSize] = (GLubyte)(color[0]);
				dst[(y * dstWidth + x) * texelSize + 1] = (GLubyte)(color[1]);
				dst[(y * dstWidth + x) * texelSize + 2] = (GLubyte)(color[2]);
			}
		}
		return true;
	}

	std::vector<GLubyte> CreateCheckboardImage(const uint32_t width, const uint32_t height, uint32_t patternSize) {
		std::vector<GLubyte> pixels;
		pixels.resize(width * height * 3);
		for (uint32_t y = 0; y < height; y++) {
			for (uint32_t x = 0; x < width; x++) {
				GLubyte color;

				if ((x / patternSize) % 2 == 0) {
					color = 255 * ((y / patternSize) % 2);
				}
				else {
					color = 255 * (1 - ((y / patternSize) % 2));
				}

				pixels[(y * height + x) * 3] = color;
				pixels[(y * height + x) * 3 + 1] = color;
				pixels[(y * height + x) * 3 + 2] = color;
			}
		}
		return pixels;
	}

	bool CreateTexture(uint32_t width, uint32_t height) {
		std::vector<GLubyte> imagePixels;

		imagePixels = CreateCheckboardImage(width, height, 8);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &mTextureId);
		glBindTexture(GL_TEXTURE_2D, mTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imagePixels.data());

		uint32_t level = 1;
		std::vector<GLubyte> prevImagePixels{ imagePixels };
		uint32_t w = width;
		uint32_t h = height;
		while (w > 1 && h > 1) {
			uint32_t newWidth;
			uint32_t newHeight;
			std::vector<GLubyte> newImagePixels;
			GenMipmap2D(prevImagePixels, newImagePixels, w, h, newWidth, newHeight);

			glTexImage2D(GL_TEXTURE_2D, level, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, newImagePixels.data());

			prevImagePixels = newImagePixels;
			w = newWidth;
			h = newHeight;

			level++;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return true;
	}


	bool InitializeShaders() {
		const char* fragmentShaderSource = SHADER_SOURCE
		(
			precision mediump float;
			varying vec2 v_texcoord;
			uniform sampler2D u_texture;
			void main (void)
			{
				
				gl_FragColor = texture2D(u_texture, v_texcoord);
			}
		);
		const char* vertexShaderSource = SHADER_SOURCE
		(
			attribute highp vec4 a_position;
			attribute lowp vec2 a_texcoord;
			varying lowp vec2 v_texcoord;
			uniform mediump float u_offset;
			void main(void)
			{
				gl_Position = a_position;
				gl_Position.x += u_offset;
				v_texcoord = a_texcoord;
			}
		);
		mShaderProgram = tinygles::CompileProgram(vertexShaderSource, fragmentShaderSource, [](GLenum type, const char * errorMessage) {
			if (errorMessage) {
				if (type == GL_VERTEX_SHADER) {
					Log(tinygles::Logger::Error, "Failed compile vertex shader : %s", errorMessage);
				}
				else if (type == GL_FRAGMENT_SHADER) {
					Log(tinygles::Logger::Error, "Failed compile fragment shader : %s", errorMessage);
				}
				else {
					Log(tinygles::Logger::Error, "Failed compile ling program : %s", errorMessage);
				}
			}
		}); 
		if (mShaderProgram == 0) {
			return false;
		}

		glUseProgram(mShaderProgram);
		GLenum lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return false; }
		mPositionAttributePos = glGetAttribLocation(mShaderProgram, "a_position");
		mTexcoordAttributePos = glGetAttribLocation(mShaderProgram, "a_texcoord");
		mOffsetPos = glGetUniformLocation(mShaderProgram, "u_offset");
		mSamplerPos = glGetUniformLocation(mShaderProgram, "s_texture");

		return true;
	}

private:
	uint32_t mWindowWidth;
	uint32_t mWindowHeight;
	float mAspect;

	tinygles::CubeGeometry mCube;

	GLuint mShaderProgram = 0;

	GLuint mPositionAttributePos = 0;
	GLuint mTexcoordAttributePos = 0;
	GLuint mSamplerPos = 0;
	GLuint mOffsetPos = 0;

	GLuint mTextureId;
};


extern "C" tinygles::Application * CreateApplication() {
	return new SimpleMinmapping();
}