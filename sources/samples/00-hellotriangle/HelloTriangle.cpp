#include "Application.h"
#include "Renderer.h"
#include "TransformHelper.h"
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

		GLfloat vertexData[] = {
			-0.4f, -0.4f, 0.0f, // Bottom Left
			0.4f, -0.4f, 0.0f, // Bottom Right
			0.0f, 0.4f, 0.0f // Top Middle
		};
		mVertexBufferHandle = renderer->CreateVertexBuffer(vertexData, sizeof(vertexData));
		
		const char* fragmentShaderSource = SHADER_SOURCE
		(
			void main(void)
			{
				gl_FragColor = vec4(1.0, 1.0, 0.66, 1.0);
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

		ShaderHandle fsHandle = renderer->CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
		ShaderHandle vsHandle = renderer->CreateShader(GL_VERTEX_SHADER, vertexShaderSource);
		mProgramHandle = renderer->CreateProgram(vsHandle, fsHandle, true);

		renderer->SetProgram(mProgramHandle);
		glBindAttribLocation(mProgramHandle.mHandle, mPositionAttributePos, "a_position");
	}

	void RenderFrame() override {
		glm::mat4 MVP = mTransformHelper.GetModelViewProjectionMatrix();

		GLenum lastError;

		Renderer* renderer = Renderer::GetRenderer();
		
		renderer->SetViewport(0, 0, mWindowWidth, mWindowHeight);

		renderer->BeginFrame();

		renderer->SetProgram(mProgramHandle);


		//@note: ProgramHandle is now a "internal id", so all this logic needs to be moved internally to the Renderer
		int matrixLocation = glGetUniformLocation(mProgramHandle.mHandle, "u_mvpMatrix");
		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &MVP[0][0]);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glEnableVertexAttribArray(mPositionAttributePos);
		glVertexAttribPointer(mPositionAttributePos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//lastError = glGetError();
		//if (lastError != GL_NO_ERROR) { return; }

		
		renderer->SetVertexBuffer(mVertexBufferHandle);
		renderer->DrawArray(0, 3);

		renderer->EndFrame();
	}

	void ReleaseView() override {
	}

	void ReleaseApplication() override {

	}

	void OnReshape(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override {
		TINYGLES_UNUSED(x); TINYGLES_UNUSED(y);
		mWindowWidth = width;
		mWindowHeight = height;
	}

private:
	ProgramHandle mProgramHandle;
	VertexBufferHandle mVertexBufferHandle;

	GLuint mPositionAttributePos = 0;

	TransformHelper mTransformHelper;

	uint32_t mWindowWidth = 0;
	uint32_t mWindowHeight = 0;
};


extern "C" tinygles::Application * CreateApplication() {
	return new HelloTriangle();
}
