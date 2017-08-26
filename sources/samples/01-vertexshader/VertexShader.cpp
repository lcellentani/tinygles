#include "Application.h"
#include "Shapes.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>

class VertexShader : public tinygles::Application {
public:
	VertexShader() = default;
	virtual ~VertexShader() {

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
		mNumIndexes = tinygles::GenerateCube(1.0f, &mVertices, nullptr, nullptr, &mIndices);

		initializeShaders(mFragmentShader, mVertexShader, mShaderProgram);
	}

	void RenderFrame() override {
		mAngle += 1.0f;
		if (mAngle > 360.0f) {
			mAngle -= 360.0f;
		}

		glm::mat4 P = glm::perspective(glm::radians(60.0f), mAspect, 0.1f, 100.0f);
		glm::mat4 V = glm::lookAt(glm::vec3(-5.0f, 2.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 M = glm::rotate(glm::mat4(1.0f), glm::radians(mAngle), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 MVP = P * V * M;

		GLenum lastError;

		glClearColor(0.36f, 0.36f, 0.36f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUniformMatrix4fv(mMVPUniformPos, 1, GL_FALSE, &MVP[0][0]);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glVertexAttribPointer(mPositionAttributPos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), mVertices);
		glEnableVertexAttribArray(mPositionAttributPos);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glDrawElements(GL_TRIANGLES, mNumIndexes, GL_UNSIGNED_INT, mIndices);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glFinish();
	}

	void ReleaseView() override {
		glDeleteShader(mFragmentShader);
		glDeleteShader(mVertexShader);
		glDeleteProgram(mShaderProgram);

		//glDeleteBuffers(1, &mVertexBuffer);
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
	bool initializeShaders(GLuint& fragmentShader, GLuint& vertexShader, GLuint& shaderProgram) {
		const char* const fragmentShaderSource = "\
											 void main (void)\
											 {\
											 gl_FragColor = vec4(1.0, 0.0, 0.0 ,1.0);\
											 }";

		// Create a fragment shader object
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Load the source code into it
		glShaderSource(fragmentShader, 1, (const char**)&fragmentShaderSource, (const GLint*)0);

		// Compile the source code
		glCompileShader(fragmentShader);

		// Check that the shader compiled
		GLint isShaderCompiled;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiled);
		if (!isShaderCompiled) {
			// If an error happened, first retrieve the length of the log message
			int infoLogLength, charactersWritten;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);

			// Allocate enough space for the message and retrieve it
			char* infoLog = new char[infoLogLength];
			glGetShaderInfoLog(fragmentShader, infoLogLength, &charactersWritten, infoLog);
			delete[] infoLog;

			return false;
		}

		// Vertex shader code
		const char* const vertexShaderSource = "\
										   attribute highp vec4	a_position;\
										   uniform mediump mat4	u_mvpMatrix;\
										   void main(void)\
										   {\
										   gl_Position = u_mvpMatrix * a_position;\
										   }";

		// Create a vertex shader object
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Load the source code into the shader
		glShaderSource(vertexShader, 1, (const char**)&vertexShaderSource, (const GLint*)0);

		// Compile the shader
		glCompileShader(vertexShader);

		// Check the shader has compiled
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isShaderCompiled);
		if (!isShaderCompiled) {
			// If an error happened, first retrieve the length of the log message
			int infoLogLength, charactersWritten;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);

			// Allocate enough space for the message and retrieve it
			char* infoLog = new char[infoLogLength];
			glGetShaderInfoLog(vertexShader, infoLogLength, &charactersWritten, infoLog);

			// Display the error in a dialog box
			delete[] infoLog;
			return false;
		}

		// Create the shader program
		shaderProgram = glCreateProgram();

		// Attach the fragment and vertex shaders to it
		glAttachShader(shaderProgram, fragmentShader);
		glAttachShader(shaderProgram, vertexShader);

		// Link the program
		glLinkProgram(shaderProgram);

		// Check if linking succeeded in the same way we checked for compilation success
		GLint isLinked;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
		if (!isLinked) {
			// If an error happened, first retrieve the length of the log message
			int infoLogLength, charactersWritten;
			glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

			// Allocate enough space for the message and retrieve it
			char* infoLog = new char[infoLogLength];
			glGetProgramInfoLog(shaderProgram, infoLogLength, &charactersWritten, infoLog);
			delete[] infoLog;

			return false;
		}

		glUseProgram(shaderProgram);
		GLenum lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return false; }

		mPositionAttributPos = glGetAttribLocation(shaderProgram, "a_position");
		mMVPUniformPos = glGetUniformLocation(shaderProgram, "u_mvpMatrix");

		return true;
	}

private:
	uint32_t mWindowWidth;
	uint32_t mWindowHeight;
	float mAspect;

	float* mVertices = nullptr;
	uint32_t* mIndices = nullptr;
	uint32_t mNumIndexes = 0;

	GLuint mFragmentShader = 0;
	GLuint mVertexShader = 0;
	GLuint mShaderProgram = 0;

	GLuint mPositionAttributPos = 0;
	GLuint mMVPUniformPos = 0;

	float mAngle = 0;
};


extern "C" tinygles::Application * CreateApplication() {
	return new VertexShader();
}