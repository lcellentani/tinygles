#include "Application.h"
#include "GeometryUtil.h"
#include "ShadersUtil.h"
#include "Log.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace tinyngine;

class VertexShader : public Application {
public:
	VertexShader() = default;
	virtual ~VertexShader() {

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

		//GenerateCube(1.0f, mCube);

		//initializeShaders(mShaderProgram);
	}

	void RenderFrame(std::unique_ptr<Renderer>& renderer) override {
		TINYNGINE_UNUSED(renderer);

		/*mAngle += 1.0f;
		if (mAngle > 360.0f) {
			mAngle -= 360.0f;
		}

		glm::mat4 P = glm::perspective(glm::radians(60.0f), mAspect, 0.1f, 100.0f);
		glm::mat4 V = glm::lookAt(glm::vec3(-2.0f, 2.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 M = glm::rotate(glm::mat4(1.0f), glm::radians(mAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 MVP = P * V * M;

		GLenum lastError;

		glClearColor(0.36f, 0.36f, 0.36f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUniformMatrix4fv(mMVPUniformPos, 1, GL_FALSE, &MVP[0][0]);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glVertexAttribPointer(mPositionAttributePos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void*)&mCube.positions[0]);
		glEnableVertexAttribArray(mPositionAttributePos);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glDrawElements(GL_TRIANGLES, mCube.numIndices, GL_UNSIGNED_INT, (const void*)&mCube.indices[0]);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glFinish();*/
	}

	void ReleaseView(std::unique_ptr<Renderer>& renderer) override {
		TINYNGINE_UNUSED(renderer);
		//glDeleteProgram(mShaderProgram);
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
	/*bool initializeShaders(GLuint& shaderProgram) {
		const char* fragmentShaderSource = SHADER_SOURCE
		(
			void main (void)
			{
				gl_FragColor = vec4(1.0, 0.0, 0.0 ,1.0);
			}
		);
		const char* vertexShaderSource = SHADER_SOURCE
		(
			attribute highp vec4 a_position;
			uniform mediump mat4 u_mvpMatrix;
			void main(void)
			{
				gl_Position = u_mvpMatrix * a_position;
			}
		);
		shaderProgram = CompileProgram(vertexShaderSource, fragmentShaderSource, [](GLenum type, const char * errorMessage) {
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
		if (shaderProgram == 0) {
			return false;
		}

		glUseProgram(shaderProgram);
		GLenum lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return false; }
		mPositionAttributePos = glGetAttribLocation(shaderProgram, "a_position");
		mMVPUniformPos = glGetUniformLocation(shaderProgram, "u_mvpMatrix");

		return true;
	}*/

private:
	uint32_t mWindowWidth;
	uint32_t mWindowHeight;
	float mAspect;

	CubeGeometry mCube;

	//GLuint mShaderProgram = 0;

	//GLuint mPositionAttributePos = 0;
	//GLuint mMVPUniformPos = 0;

	float mAngle = 0;
};


extern "C" tinyngine::Application * CreateApplication() {
	return new VertexShader();
}