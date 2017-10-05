#include "Application.h"
#include "GeometryUtil.h"
#include "Log.h"

#include <vector>

using namespace tinyngine;

class SimpleTexture : public Application {
public:
	SimpleTexture() = default;
	virtual ~SimpleTexture() {

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
		TINYNGINE_UNUSED(renderer);

		//InitializeShaders();

		//CreateSimpleTexture();
	}

	void RenderFrame(std::unique_ptr<Renderer>& renderer, float deltaTime) override {
		TINYNGINE_UNUSED(deltaTime);
		TINYNGINE_UNUSED(renderer);
		/*const GLfloat vertices[]{
			-0.5f, 0.5f, 0.0f,
			0.0f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.0f, 1.0f,
			0.5f, -0.5f, 0.0f,
			1.0f, 1.0f,
			0.5f, 0.5f, 0.0f,
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

		glVertexAttribPointer(mPositionAttributePos, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*)&vertices[0]);
		glEnableVertexAttribArray(mPositionAttributePos);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glVertexAttribPointer(mTexcoordAttributePos, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*)&vertices[3]);
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

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void*)&indices[0]);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glFinish();*/
	}

	void ReleaseView(std::unique_ptr<Renderer>& renderer) override {
		TINYNGINE_UNUSED(renderer);
		//glDeleteProgram(mShaderProgram);
		//glDeleteTextures(1, &mTextureId);
	}

	void ReleaseApplication() override {

	}

	void OnReshape(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override {
		TINYNGINE_UNUSED(x); TINYNGINE_UNUSED(y);
		mWindowWidth = width;
		mWindowHeight = height;
		mAspect = (float)mWindowWidth / (float)mWindowHeight;
	}

private:
	/*bool CreateSimpleTexture() {
		const GLubyte pixels[4 * 3]{
			255, 0, 0,
			0, 255, 0,
			0, 0, 255,
			255, 255, 0
		};

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &mTextureId);
		glBindTexture(GL_TEXTURE_2D, mTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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
			void main(void)
			{
				gl_Position = a_position;
				v_texcoord = a_texcoord;
			}
		);
		mShaderProgram = CompileProgram(vertexShaderSource, fragmentShaderSource, [](GLenum type, const char * errorMessage) {
			if (errorMessage) {
				if (type == GL_VERTEX_SHADER) {
					Log(Logger::Error, "Failed compile vertex shader : %s", errorMessage);
				}
				else if (type == GL_FRAGMENT_SHADER) {
					Log(Logger::Error, "Failed compile fragment shader : %s", errorMessage);
				}
				else {
					Log(Logger::Error, "Failed compile ling program : %s", errorMessage);
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
		mSamplerPos = glGetUniformLocation(mShaderProgram, "s_texture");

		return true;
	}*/

private:
	uint32_t mWindowWidth;
	uint32_t mWindowHeight;
	float mAspect;

	//GLuint mShaderProgram = 0;

	//GLuint mPositionAttributePos = 0;
	//GLuint mTexcoordAttributePos = 0;
	//GLuint mSamplerPos = 0;

	//GLuint mTextureId;
};


extern "C" tinyngine::Application * CreateApplication() {
	return new SimpleTexture();
}