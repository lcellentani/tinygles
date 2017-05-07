#include "Application.h"

#include <memory>

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>

class HelloTriangle : public tinygles::Application {
public:
	HelloTriangle()
		: mAttributes()
		, mVertexBuffer(0)
		, mFragmentShader(0)
		, mVertexShader(0)
		, mShaderProgram(0)
		, mVertexArray(0)
		, mWindowWidth(0)
		, mWindowHeight(0) {
		mAttributes.mRequiredApi = tinygles::Api::OpenGLES2;
		mAttributes.mDepthBPP = 32;
		mAttributes.mStencilBPP = 0;
		mAttributes.mRedBits = 8;
		mAttributes.mGreenBits = 8;
		mAttributes.mBlueBits = 8;
		mAttributes.mAlphaBits = 8;
	}
	virtual ~HelloTriangle() {

	}

	tinygles::ContextAttribs& GetContextAttribs() override {
		return mAttributes;
	}

	void InitApplication() override {

	}

	void InitView() override {
		initializeBuffer(mVertexBuffer);
		initializeShaders(mFragmentShader, mVertexShader, mShaderProgram);
	}

	void RenderFrame() override {
		// Matrix used to specify the orientation of the triangle on screen.
		static const float transformationMatrix[] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		GLenum lastError;

		glViewport(0, 0, mWindowWidth, mWindowHeight);

		glClearColor(0.36f, 0.36f, 0.36f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Get the location of the transformation matrix in the shader using its name
		int matrixLocation = glGetUniformLocation(mShaderProgram, "u_mvpMatrix");
		// Pass the transformationMatrix to the shader using its location
		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, transformationMatrix);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		// Enable the user-defined vertex array
		glEnableVertexAttribArray(mVertexArray);

		// Sets the vertex data to this attribute index, with the number of floats in each position
		glVertexAttribPointer(mVertexArray, 3, GL_FLOAT, GL_FALSE, 0, 0);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glDrawArrays(GL_TRIANGLES, 0, 3);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glFinish();
	}

	void ReleaseView() override {
		glDeleteShader(mFragmentShader);
		glDeleteShader(mVertexShader);
		glDeleteProgram(mShaderProgram);

		glDeleteBuffers(1, &mVertexBuffer);
	}

	void ReleaseApplication() override {

	}

	void OnReshape(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override {
		mWindowWidth = width;
		mWindowHeight = height;
	}

private:
	bool initializeBuffer(GLuint& vertexBuffer) {
		// Vertex data containing the positions of each point of the triangle
		GLfloat vertexData[] = {
			-0.4f, -0.4f, 0.0f, // Bottom Left
			0.4f, -0.4f, 0.0f, // Bottom Right
			0.0f, 0.4f, 0.0f // Top Middle
		};

		// Generate a buffer object
		glGenBuffers(1, &vertexBuffer);

		// Bind buffer as an vertex buffer so we can fill it with data
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		//	Set the buffer's size, data and usage
		//	Note the last argument - GL_STATIC_DRAW. This tells the driver that we intend to read from the buffer on the GPU, and don't intend
		//	to modify the data until we're done with it.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		GLenum lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return false; }

		return true;
	}

	bool initializeShaders(GLuint& fragmentShader, GLuint& vertexShader, GLuint& shaderProgram) {
		const char* const fragmentShaderSource = "\
											 void main (void)\
											 {\
												gl_FragColor = vec4(1.0, 1.0, 0.66 ,1.0);\
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
	tinygles::ContextAttribs mAttributes;

	GLuint mVertexBuffer;
	GLuint mFragmentShader;
	GLuint mVertexShader;
	GLuint mShaderProgram;

	GLuint mVertexArray;

	uint32_t mWindowWidth;
	uint32_t mWindowHeight;
};


extern "C" tinygles::Application * CreateApplication() {
	return new HelloTriangle();
}