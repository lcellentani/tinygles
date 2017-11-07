#include "Application.h"
#include "GraphicsDevice.h"
#include "VertexFormat.h"
#include "TransformHelper.h"
#include "Log.h"

using namespace tinyngine;

class HelloTriangle : public Application {
public:
	HelloTriangle() = default;
	virtual ~HelloTriangle() = default;

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

		GraphicsDevice& graphicsDevice = engine.GetSystem<GraphicsDevice>();

		mPosVertexFormat.Add(Attributes::Position, AttributeType::Float, 3, false);
		
		mVertexBufferHandle = graphicsDevice.CreateVertexBuffer(cVertexData, sizeof(cVertexData), mPosVertexFormat);
		
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

		ShaderHandle vsHandle = graphicsDevice.CreateShader(ShaderType::VertexProgram, vertexShaderSource);
		ShaderHandle fsHandle = graphicsDevice.CreateShader(ShaderType::FragmentProgram, fragmentShaderSource);
		
		mProgramHandle = graphicsDevice.CreateProgram(vsHandle, fsHandle, true);
		mModelViewProjHandle = graphicsDevice.GetUniform(mProgramHandle, "u_modelViewProj");
	}

	void RenderFrame(Engine& engine) override {
		GraphicsDevice& graphicsDevice = engine.GetSystem<GraphicsDevice>();

		glm::mat4 modelViewProj = mTransformHelper.GetModelViewProjectionMatrix();

		graphicsDevice.Clear(GraphicsDevice::ColorBuffer, Color(92, 92, 92));

		graphicsDevice.SetVertexBuffer(mVertexBufferHandle, Attributes::Position);

		graphicsDevice.SetProgram(mProgramHandle, mPosVertexFormat);
		graphicsDevice.SetUniformMat4(mProgramHandle, mModelViewProjHandle, &modelViewProj[0][0], false);
	
		graphicsDevice.DrawArray(PrimitiveType::Triangles, 0, 3);

		graphicsDevice.Commit();
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
