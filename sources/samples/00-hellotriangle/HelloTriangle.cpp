#include "Application.h"
#include "Renderer.h"
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

	void InitView(Engine& engine, uint32_t, uint32_t) override {
		const float cVertexData[] = {
			-0.4f, -0.4f, 0.0f, // Bottom Left
			0.4f, -0.4f, 0.0f, // Bottom Right
			0.0f, 0.4f, 0.0f // Top Middle
		};

		Renderer& renderer = engine.GetSystem<Renderer>();

		mPosVertexFormat.Add(Attributes::Position, AttributeType::Float, 3, false);
		
		mVertexBufferHandle = renderer.CreateVertexBuffer(cVertexData, sizeof(cVertexData), mPosVertexFormat);
		
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

		ShaderHandle vsHandle = renderer.CreateShader(ShaderType::VertexProgram, vertexShaderSource);
		ShaderHandle fsHandle = renderer.CreateShader(ShaderType::FragmentProgram, fragmentShaderSource);
		
		mProgramHandle = renderer.CreateProgram(vsHandle, fsHandle, true);
		mModelViewProjHandle = renderer.GetUniform(mProgramHandle, "u_modelViewProj");
	}

	void RenderFrame(Engine& engine) override {
		Renderer& renderer = engine.GetSystem<Renderer>();

		glm::mat4 modelViewProj = mTransformHelper.GetModelViewProjectionMatrix();

		renderer.Clear(Renderer::ClearFlags::ColorBuffer, Color(92, 92, 92));

		renderer.SetVertexBuffer(mVertexBufferHandle, Attributes::Position);

		renderer.SetProgram(mProgramHandle, mPosVertexFormat);
		renderer.SetUniformMat4(mProgramHandle, mModelViewProjHandle, &modelViewProj[0][0], false);
	
		renderer.DrawArray(PrimitiveType::Triangles, 0, 3);

		renderer.Commit();
	}

	void ReleaseView(Engine&) override {
	}

	void ReleaseApplication() override {

	}

private:
	VertexFormat mPosVertexFormat;
	ProgramHandle mProgramHandle;
	UniformHandle mModelViewProjHandle;
	VertexBufferHandle mVertexBufferHandle;

	TransformHelper mTransformHelper;
};


extern "C" tinyngine::Application * CreateApplication() {
	return new HelloTriangle();
}
