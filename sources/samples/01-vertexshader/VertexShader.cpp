#include "Application.h"
#include "GraphicsDevice.h"
#include "MeshLoader.h"
#include "VertexFormat.h"
#include "TransformHelper.h"
#include "Log.h"

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace tinyngine;

class VertexShader : public Application {
public:
	VertexShader() = default;
	virtual ~VertexShader() = default;

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

	void InitView(Engine& engine, uint32_t windowWidth, uint32_t windowHeight) override {
		GraphicsDevice& graphicsDevice = engine.GetSystem<GraphicsDevice>();
		MeshLoader& meshLoader = engine.GetSystem<MeshLoader>();
		mCube = meshLoader.GenerateCube(1.0f);
		mColors.reserve(mCube.mNumVertices * 4);
		for (uint32_t i = 0; i < mCube.mNumVertices * 4; i += 4) {
			mColors.push_back(255);
			mColors.push_back(255);
			mColors.push_back(0);
			mColors.push_back(255);
		}

		mPosVertexFormat.Add(Attributes::Position, AttributeType::Float, 3, false);
		mPosVertexFormat.Add(Attributes::Color0, AttributeType::Uint8, 4, true);

		mPositionsHandle = graphicsDevice.CreateVertexBuffer(&mCube.mPositions[0], sizeof(mCube.mPositions[0]) * mCube.mNumVertices * 3, mPosVertexFormat);
		mColorsHandle = graphicsDevice.CreateVertexBuffer(&mColors[0], sizeof(mColors[0]) * mColors.size() * 4, mPosVertexFormat);
		mIndexesBufferHandle = graphicsDevice.CreateIndexBuffer(&mCube.mIndices[0], sizeof(mCube.mIndices[0]) * mCube.mNumIndices);

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
			attribute lowp vec4 a_color0;
			uniform mediump mat4 u_modelViewProj;
			varying lowp vec4 v_color;
			void main(void)
			{
				gl_Position = u_modelViewProj * a_position;
				v_color = a_color0;
			}
		);
		ShaderHandle vsHandle = graphicsDevice.CreateShader(ShaderType::VertexProgram, vertexShaderSource);
		ShaderHandle fsHandle = graphicsDevice.CreateShader(ShaderType::FragmentProgram, fragmentShaderSource);
		mProgramHandle = graphicsDevice.CreateProgram(vsHandle, fsHandle, true);
		mModelViewProjHandle = graphicsDevice.GetUniform(mProgramHandle, "u_modelViewProj");

		float ratio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
		mProj = glm::perspective(glm::radians(60.0f), ratio, 0.1f, 100.0f);
		mView = glm::lookAt(glm::vec3(-2.0f, 2.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		mUp = glm::vec3(0.0f, 1.0f, 0.0f);
		mRight = glm::vec3(1.0f, 0.0f, 0.0f);
		mAngles = glm::vec3(0.0f, 0.0f, 0.0f);

		mTransformHelper.SetMatrixMode(TransformHelper::MatrixMode::Projection);;
		mTransformHelper.LoadMatrix(mProj);
		mTransformHelper.SetMatrixMode(TransformHelper::MatrixMode::View);
		mTransformHelper.LoadMatrix(mView);

		graphicsDevice.SetState(RendererStateType::CullFace, true);
	}

	void RenderFrame(Engine& engine) override {
		GraphicsDevice& graphicsDevice = engine.GetSystem<GraphicsDevice>();
		mAngles.x -= mSpeed.x;
		if (mAngles.x < 0.0f) {
			mAngles.x += 360.0f;
		}
		mAngles.y += mSpeed.y;
		if (mAngles.y > 360.0f) {
			mAngles.y -= 360.0f;
		}
		
		mTransformHelper.SetMatrixMode(TransformHelper::MatrixMode::Model);
		mTransformHelper.LoadIdentity();
		mTransformHelper.Rotate(mAngles.y, mUp);
		mTransformHelper.Rotate(-mAngles.x, mRight);

		graphicsDevice.Clear(GraphicsDevice::ColorBuffer | GraphicsDevice::DepthBuffer, Color(92, 92, 92));

		graphicsDevice.SetVertexBuffer(mPositionsHandle, Attributes::Position);
		graphicsDevice.SetVertexBuffer(mColorsHandle, Attributes::Color0);

		graphicsDevice.SetProgram(mProgramHandle, mPosVertexFormat);
		graphicsDevice.SetUniformMat4(mProgramHandle, mModelViewProjHandle, &mTransformHelper.GetModelViewProjectionMatrix()[0][0], false);

		graphicsDevice.SetIndexBuffer(mIndexesBufferHandle);
		graphicsDevice.DrawElements(PrimitiveType::Triangles, mCube.mNumIndices);

		graphicsDevice.Commit();
	}

	void ReleaseView(Engine&) override {
	}

	void ReleaseApplication() override {

	}

private:
	MeshInfo mCube;
	std::vector<uint8_t> mColors;

	VertexFormat mPosVertexFormat;
	ProgramHandle mProgramHandle;
	UniformHandle mModelViewProjHandle;
	VertexBufferHandle mPositionsHandle;
	VertexBufferHandle mColorsHandle;
	IndexBufferHandle mIndexesBufferHandle;

	TransformHelper mTransformHelper;

	glm::vec3 mSpeed{ 2.f, 1.f, 0.0f };
	glm::vec3 mAngles;
	glm::mat4 mProj;
	glm::mat4 mView;
	glm::vec3 mRight;
	glm::vec3 mUp;
};


extern "C" tinyngine::Application * CreateApplication() {
	return new VertexShader();
}