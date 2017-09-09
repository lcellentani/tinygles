#include "Application.h"
#include "Renderer.h"
#include "Log.h"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>

using namespace tinygles;

class HelloTriangle : public Application {
public:
	HelloTriangle() {
	}

	virtual ~HelloTriangle() {

	}

	tinygles::ContextAttribs& GetContextAttribs() override {
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

	void InitView() override {
		Renderer* renderer = Renderer::GetRenderer();
		renderer->SetViewClear(ClearFlags::Color, Color(92, 92, 92));

		initializeBuffer(mVertexBuffer);
		initializeShaders();
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

		Renderer* renderer = Renderer::GetRenderer();
		
		renderer->SetViewport(0, 0, mWindowWidth, mWindowHeight);

		renderer->BeginFrame();

		// Get the location of the transformation matrix in the shader using its name
		int matrixLocation = glGetUniformLocation(mProgramHandle.mHandle, "u_mvpMatrix");
		// Pass the transformationMatrix to the shader using its location
		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, transformationMatrix);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glEnableVertexAttribArray(mPositionAttributePos);
		glVertexAttribPointer(mPositionAttributePos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glDrawArrays(GL_TRIANGLES, 0, 3);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		renderer->EndFrame();
	}

	void ReleaseView() override {
		glDeleteBuffers(1, &mVertexBuffer);
	}

	void ReleaseApplication() override {

	}

	void OnReshape(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override {
		TINYGLES_UNUSED(x); TINYGLES_UNUSED(y);
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

	bool initializeShaders() {
		Renderer* renderer = Renderer::GetRenderer();
		
		const char* fragmentShaderSource = SHADER_SOURCE
		(
			void main (void)
			{
				gl_FragColor = vec4(1.0, 1.0, 0.66 ,1.0);
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
		
		mProgramHandle = renderer->CreateProgram(vertexShaderSource, fragmentShaderSource, [](uint32_t type, const char * errorMessage) {
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

		glBindAttribLocation(mProgramHandle.mHandle, mPositionAttributePos, "a_position");
		glUseProgram(mProgramHandle.mHandle);
		
		return true;
	}

private:
	GLuint mVertexBuffer = 0;
	ProgramHandle mProgramHandle;

	GLuint mPositionAttributePos = 0;

	uint32_t mWindowWidth = 0;
	uint32_t mWindowHeight = 0;
};


extern "C" tinygles::Application * CreateApplication() {
	return new HelloTriangle();
}
