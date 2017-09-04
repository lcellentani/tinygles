#include "Application.h"
#include "GeometryUtil.h"
#include "ShadersUtil.h"
#include "Log.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>

class DirectionalLighting : public tinygles::Application {
public:
	DirectionalLighting() = default;
	virtual ~DirectionalLighting() {

	}

	tinygles::ContextAttribs& GetContextAttribs() override {
		static tinygles::ContextAttribs sAttributes;

		sAttributes.mRequiredApi = tinygles::Api::OpenGLES2;
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

	void InitView() override {
		tinygles::GenerateCube(1.0f, mCube);

		mColors.reserve(mCube.numVertices);
		for (uint32_t i = 0; i < mCube.numVertices * 4; i += 4) {
			mColors.push_back(255);
			mColors.push_back(255);
			mColors.push_back(0);
			mColors.push_back(255);
		}

		initializeShaders(mShaderProgram);
	}

	void RenderFrame() override {
		mAngle += 1.0f;
		if (mAngle > 360.0f) {
			mAngle -= 360.0f;
		}

		glm::mat4 P = glm::perspective(glm::radians(60.0f), mAspect, 0.1f, 100.0f);
		glm::mat4 V = glm::lookAt(glm::vec3(-2.0f, 2.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 M = glm::rotate(glm::mat4(1.0f), glm::radians(mAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 MVP = P * V * M;
		glm::mat4 MV = V * M;

		GLenum lastError;

		glClearColor(0.36f, 0.36f, 0.36f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUniformMatrix4fv(mMVPUniformPos, 1, GL_FALSE, &MVP[0][0]);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }
		glUniformMatrix4fv(mMVUniformPos, 1, GL_FALSE, &MV[0][0]);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glVertexAttribPointer(mPositionAttributePos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void*)&mCube.positions[0]);
		glEnableVertexAttribArray(mPositionAttributePos);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }
		glVertexAttribPointer(mNormalAttributePos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void*)&mCube.normals[0]);
		glEnableVertexAttribArray(mNormalAttributePos);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }
		glVertexAttribPointer(mColorAttributePos, 3, GL_UNSIGNED_BYTE, GL_TRUE, 4 * sizeof(GLubyte), (const void*)&mColors[0]);
		glEnableVertexAttribArray(mColorAttributePos);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glDrawElements(GL_TRIANGLES, mCube.numIndices, GL_UNSIGNED_INT, (const void*)&mCube.indices[0]);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glFinish();
	}

	void ReleaseView() override {
		glDeleteProgram(mShaderProgram);
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
	bool initializeShaders(GLuint& shaderProgram) {
		const char* fragmentShaderSource = SHADER_SOURCE
		(
			varying lowp vec4 v_color;
			void main(void)
			{
				gl_FragColor = v_color;
			}
		);
		const char* vertexShaderSource = SHADER_SOURCE
		(
			attribute highp vec4 a_position;
			attribute highp vec3 a_normal;
			attribute lowp vec4 a_color;
			uniform mediump mat4 u_mvpMatrix;
			uniform mediump mat4 u_mvMatrix;
			uniform mediump vec3 u_lightPos;
			varying lowp vec4 v_color;
			void main(void)
			{
				vec3 modelViewPos = vec3(u_mvMatrix * a_position);
				vec3 modelViewNormal = vec3(u_mvMatrix * vec4(a_normal, 0.0));
				float distance = length(u_lightPos - modelViewPos);
				vec3 lightVector = normalize(u_lightPos - modelViewPos);
				float diffuse = max(dot(modelViewNormal, lightVector), 0.1);
				diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));
				v_color = a_color * diffuse;

				gl_Position = u_mvpMatrix * a_position;
			}
		);
		shaderProgram = tinygles::CompileProgram(vertexShaderSource, fragmentShaderSource, [](GLenum type, const char * errorMessage) {
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
		if (shaderProgram == 0) {
			return false;
		}

		glUseProgram(shaderProgram);
		GLenum lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return false; }
		mPositionAttributePos = glGetAttribLocation(shaderProgram, "a_position");
		mColorAttributePos = glGetAttribLocation(shaderProgram, "a_color");
		mNormalAttributePos = glGetAttribLocation(shaderProgram, "a_normal");
		mMVPUniformPos = glGetUniformLocation(shaderProgram, "u_mvpMatrix");
		mMVUniformPos = glGetUniformLocation(shaderProgram, "u_mvMatrix");
		mLighPositionPos = glGetUniformLocation(shaderProgram, "u_lightPos");

		glEnable(GL_CULL_FACE);

		return true;
	}

private:
	uint32_t mWindowWidth;
	uint32_t mWindowHeight;
	float mAspect;

	tinygles::CubeGeometry mCube;
	std::vector<GLubyte> mColors;

	GLuint mShaderProgram = 0;

	GLuint mPositionAttributePos = 0;
	GLuint mColorAttributePos = 0;
	GLuint mNormalAttributePos = 0;
	GLuint mMVPUniformPos = 0;
	GLuint mMVUniformPos = 0;
	GLuint mLighPositionPos = 0;

	float mAngle = 0;
};


extern "C" tinygles::Application * CreateApplication() {
	return new DirectionalLighting();
}