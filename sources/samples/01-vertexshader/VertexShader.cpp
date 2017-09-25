#include "Application.h"
#include "GeometryUtil.h"
#include "VertexFormat.h"
#include "TransformHelper.h"
#include "Log.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace tinyngine;

class VertexShader : public Application {
public:
	VertexShader() = default;
	virtual ~VertexShader() {

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
		GenerateCube(1.0f, mCube);

		mPosVertexFormat.Add(Attributes::Position, AttributeType::Float, 3, false);

		mVertexBufferHandle = renderer->CreateVertexBuffer(&mCube.positions[0], sizeof(mCube.positions[0]) * mCube.numVertices * 3, mPosVertexFormat);
		mIndexBufferHandle = renderer->CreateIndexBuffer(&mCube.indices[0], sizeof(mCube.indices[0]) * mCube.numIndices);

		const char* fragmentShaderSource = SHADER_SOURCE
		(
			void main(void)
			{
				gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
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

		mProj = glm::perspective(glm::radians(60.0f), mAspect, 0.1f, 100.0f);
		mView = glm::lookAt(glm::vec3(-2.0f, 2.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		mUp = glm::vec3(0.0f, 1.0f, 0.0f);
		mRight = glm::vec3(1.0f, 0.0f, 0.0f);
		mAngles = glm::vec3(0.0f, 0.0f, 0.0f);

		mTransformHelper.SetMatrixMode(TransformHelper::MatrixMode::Projection);;
		mTransformHelper.LoadMatrix(mProj);
		mTransformHelper.SetMatrixMode(TransformHelper::MatrixMode::View);
		mTransformHelper.LoadMatrix(mView);
	}

	void RenderFrame(std::unique_ptr<Renderer>& renderer) override {
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

		renderer->SetViewport(0, 0, mWindowWidth, mWindowHeight);
		renderer->Clear(Renderer::ClearFlags::ColorBuffer, Color(92, 92, 92));

		renderer->SetVertexBuffer(mVertexBufferHandle);
		renderer->SetIndexBuffer(mIndexBufferHandle);

		renderer->SetProgram(mProgramHandle, mPosVertexFormat);
		renderer->SetUniformMat4(mProgramHandle, Uniforms::ModelViewProj, &mTransformHelper.GetModelViewProjectionMatrix()[0][0], false);

		renderer->DrawElements(PrimitiveType::Triangles, mCube.numIndices);

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
		mAspect = (float)mWindowWidth / (float)mWindowHeight;
	}

private:
	uint32_t mWindowWidth;
	uint32_t mWindowHeight;
	float mAspect;

	CubeGeometry mCube;

	VertexFormat mPosVertexFormat;
	ProgramHandle mProgramHandle;
	VertexBufferHandle mVertexBufferHandle;
	IndexBufferHandle mIndexBufferHandle;

	TransformHelper mTransformHelper;

	glm::vec3 mSpeed{ 0.025f, 0.01f, 0.0f };
	glm::vec3 mAngles;
	glm::mat4 mProj;
	glm::mat4 mView;
	glm::vec3 mRight;
	glm::vec3 mUp;
};


extern "C" tinyngine::Application * CreateApplication() {
	return new VertexShader();
}