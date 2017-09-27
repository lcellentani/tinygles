#include "Application.h"
#include "VertexFormat.h"
#include "TransformHelper.h"
#include "Log.h"

using namespace tinyngine;

class HelloTriangle : public Application {
public:
	HelloTriangle() {
	}

	virtual ~HelloTriangle() {

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
		const float cVertexData[] = {
			-0.4f, -0.4f, 0.0f, // Bottom Left
			0.4f, -0.4f, 0.0f, // Bottom Right
			0.0f, 0.4f, 0.0f // Top Middle
		};

		mPosVertexFormat.Add(Attributes::Position, AttributeType::Float, 3, false);
		
		mVertexBufferHandle = renderer->CreateVertexBuffer(cVertexData, sizeof(cVertexData), mPosVertexFormat);
		
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
			uniform mediump mat4 u_modelViewProj;
			void main(void)
			{
				gl_Position = u_modelViewProj * a_position;
			}
		);

		ShaderHandle vsHandle = renderer->CreateShader(ShaderType::VertexProgram, vertexShaderSource);
		ShaderHandle fsHandle = renderer->CreateShader(ShaderType::FragmentProgram, fragmentShaderSource);
		
		mProgramHandle = renderer->CreateProgram(vsHandle, fsHandle, true);
	}

	void RenderFrame(std::unique_ptr<Renderer>& renderer, float deltaTime) override {
		TINYNGINE_UNUSED(deltaTime);
		glm::mat4 modelViewProj = mTransformHelper.GetModelViewProjectionMatrix();

		renderer->SetViewport(0, 0, mWindowWidth, mWindowHeight);
		renderer->Clear(Renderer::ClearFlags::ColorBuffer, Color(92, 92, 92));

		renderer->SetVertexBuffer(mVertexBufferHandle, Attributes::Position);

		renderer->SetProgram(mProgramHandle, mPosVertexFormat);
		renderer->SetUniformMat4(mProgramHandle, Uniforms::ModelViewProj, &modelViewProj[0][0], false);
	
		renderer->DrawArray(PrimitiveType::Triangles, 0, 3);

		renderer->Commit();
	}

	void ReleaseView(std::unique_ptr<Renderer>& renderer) override {
		TINYNGINE_UNUSED(renderer);
	}

	void ReleaseApplication() override {

	}

	void OnReshape(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override {
		TINYNGINE_UNUSED(x); TINYNGINE_UNUSED(y);
		mWindowWidth = width;
		mWindowHeight = height;
	}

private:
	VertexFormat mPosVertexFormat;
	ProgramHandle mProgramHandle;
	VertexBufferHandle mVertexBufferHandle;

	TransformHelper mTransformHelper;

	uint32_t mWindowWidth = 0;
	uint32_t mWindowHeight = 0;
};


extern "C" tinyngine::Application * CreateApplication() {
	return new HelloTriangle();
}
