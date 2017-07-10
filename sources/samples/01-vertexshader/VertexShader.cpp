#include "Application.h"
#include "Shapes.h"
#include "Mathlib.h"

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
		mat4 P = mathlib::Matrix<float, 4>::Perspective(60.0f, mAspect, 1.0f, 10.0f);
		mat4 T = mat4::FromTranslationVector(vec3(0.0f, 0.0f, -2.0f));

		mat4 MVP = T * P;

		//vec4 v(1.0f, 2.0f, 3.0f, 4.0f);
		//float a[4] = new float[4];
		//a = vec4::template ToType<float*>(v);

		//GLenum lastError;

		glClearColor(0.36f, 0.36f, 0.36f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//int matrixLocation = glGetUniformLocation(mShaderProgram, "u_mvpMatrix");
		//glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, MVP);
		//lastError = glGetError();
		//if (lastError != GL_NO_ERROR) { return; }

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

		// Bind the vertex attribute "myVertex" to location VERTEX_ARRAY (0)
		glBindAttribLocation(shaderProgram, mVertexArray, "a_position");

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
	GLuint mVertexArray = 0;
};


extern "C" tinygles::Application * CreateApplication() {
	return new VertexShader();
}