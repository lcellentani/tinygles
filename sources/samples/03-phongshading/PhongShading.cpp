#include "Application.h"
#include "GeometryUtil.h"
#include "VertexFormat.h"
#include "TransformHelper.h"
#include "StringUtils.h"
#include "Log.h"

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace
{

static float cMaterialAmbient[] = { 0.2f, 0.0f, 0.2f };
static float cLightAmbient[] = { 1.0f, 1.0f, 1.0f };
static float cMaterialDiffuse[] = { 0.9f, 0.0f, 0.0f };
static float cLightDiffuse[] = { 1.0f, 1.0f, 1.0f };
static float cMaterialSpecular[] = { 1.0f, 0.9f, 0.9f };
static float cLightSpecular[] = { 1.0f, 1.0f, 1.0f };
static float cShininessFactor = 15.0f;
static float cLightPosisiont[] = { 0.0f, 20.0f, 10.0f };

}

using namespace tinyngine;

class PhongShading : public Application {
public:
	PhongShading() = default;
	virtual ~PhongShading() {

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
		//LoadObj("models/Cube.obj", true, mObject);
		LoadObj("models/Sphere.obj", true, mObject);
		//LoadObj("models/Monkey.obj", true, mObject);
		//LoadObj("models/Teapot.obj", true, mObject);
		//LoadObj("models/Cylinder.obj", true, mObject);

		Geometry& shape = mObject.shapes[0];
		mColors.reserve(shape.numVertices * 4);
		for (uint32_t i = 0; i < shape.numVertices * 4; i += 4) {
			mColors.push_back(255);
			mColors.push_back(255);
			mColors.push_back(255);
			mColors.push_back(255);
		}

		mPosVertexFormat.Add(Attributes::Position, AttributeType::Float, 3, false);
		mPosVertexFormat.Add(Attributes::Normal, AttributeType::Float, 3, false);
		mPosVertexFormat.Add(Attributes::Color0, AttributeType::Uint8, 4, true);

		mPositionsHandle = renderer->CreateVertexBuffer(&shape.positions[0], sizeof(shape.positions[0]) * shape.numVertices * 3, mPosVertexFormat);
		mNornalsHandle = renderer->CreateVertexBuffer(&shape.normals[0], sizeof(shape.normals[0]) * shape.numVertices * 3, mPosVertexFormat);
		mColorsHandle = renderer->CreateVertexBuffer(&mColors[0], sizeof(mColors[0]) * mColors.size() * 4, mPosVertexFormat);
		mIndexesBufferHandle = renderer->CreateIndexBuffer(&shape.indices[0], sizeof(shape.indices[0]) * shape.numIndices);

		std::string vertexShaderSource;
		StringUtils::ReadFileToString("shaders/phong_vert_2.glsl", vertexShaderSource);
		std::string fragmentShaderSource;
		StringUtils::ReadFileToString("shaders/phong_frag_2.glsl", fragmentShaderSource);

		ShaderHandle vsHandle = renderer->CreateShader(ShaderType::VertexProgram, vertexShaderSource.c_str());
		ShaderHandle fsHandle = renderer->CreateShader(ShaderType::FragmentProgram, fragmentShaderSource.c_str());
		mProgramHandle = renderer->CreateProgram(vsHandle, fsHandle, true);
		mModelViewProjHandle = renderer->GetUniform(mProgramHandle, "u_modelViewProj");
		mModelViewHandle = renderer->GetUniform(mProgramHandle, "u_modelView");
		mMaterialAmbientHandle = renderer->GetUniform(mProgramHandle, "u_materialAmbient");
		mLightAmbientHandle = renderer->GetUniform(mProgramHandle, "u_lightAmbient");
		mMaterialDiffuseHandle = renderer->GetUniform(mProgramHandle, "u_materialDiffuse");
		mLightDiffuseHandle = renderer->GetUniform(mProgramHandle, "u_lightDiffuse");
		mMaterialSpecularHandle = renderer->GetUniform(mProgramHandle, "u_materialSpecular");
		mLightSpecularHandle = renderer->GetUniform(mProgramHandle, "u_lightSpecular");
		mShininessFactorHandle = renderer->GetUniform(mProgramHandle, "u_shininessFactor");
		mLightPositionHandle = renderer->GetUniform(mProgramHandle, "u_lightPosition");

		mProj = glm::perspective(glm::radians(60.0f), mAspect, 0.1f, 100.0f);
		mView = glm::lookAt(glm::vec3(-2.0f, 2.0f, -4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		mUp = glm::vec3(0.0f, 1.0f, 0.0f);
		mRight = glm::vec3(1.0f, 0.0f, 0.0f);
		mAngles = glm::vec3(0.0f, 0.0f, 0.0f);

		mTransformHelper.SetMatrixMode(TransformHelper::MatrixMode::Projection);;
		mTransformHelper.LoadMatrix(mProj);
		mTransformHelper.SetMatrixMode(TransformHelper::MatrixMode::View);
		mTransformHelper.LoadMatrix(mView);

		renderer->SetState(RendererStateType::CullFace, true);
		renderer->SetState(RendererStateType::DepthTest, true);
	}

	void RenderFrame(std::unique_ptr<Renderer>& renderer, float deltaTime) override {
		mAngles.x -= mSpeed.x * deltaTime;
		if (mAngles.x < 0.0f) {
			mAngles.x += 360.0f;
		}
		mAngles.y += mSpeed.y * deltaTime;
		if (mAngles.y > 360.0f) {
			mAngles.y -= 360.0f;
		}

		mTransformHelper.SetMatrixMode(TransformHelper::MatrixMode::Model);
		mTransformHelper.LoadIdentity();
		mTransformHelper.Rotate(mAngles.y, mUp);
		mTransformHelper.Rotate(-mAngles.x, mRight);

		renderer->SetViewport(0, 0, mWindowWidth, mWindowHeight);
		renderer->Clear(Renderer::ColorBuffer | Renderer::DepthBuffer, Color(92, 92, 92), 1.0f);

		renderer->SetVertexBuffer(mPositionsHandle, Attributes::Position);
		renderer->SetVertexBuffer(mNornalsHandle, Attributes::Normal);
		renderer->SetVertexBuffer(mColorsHandle, Attributes::Color0);

		renderer->SetProgram(mProgramHandle, mPosVertexFormat);
		renderer->SetUniformMat4(mProgramHandle, mModelViewProjHandle, &mTransformHelper.GetModelViewProjectionMatrix()[0][0], false);
		renderer->SetUniformMat4(mProgramHandle, mModelViewHandle, &mTransformHelper.GetModelViewMatrix()[0][0], false);
		renderer->SetUniformFloat3(mProgramHandle, mMaterialAmbientHandle, &cMaterialAmbient[0]);
		renderer->SetUniformFloat3(mProgramHandle, mLightAmbientHandle, &cLightAmbient[0]);
		renderer->SetUniformFloat3(mProgramHandle, mMaterialDiffuseHandle, &cMaterialDiffuse[0]);
		renderer->SetUniformFloat3(mProgramHandle, mLightDiffuseHandle, &cLightDiffuse[0]);
		renderer->SetUniformFloat3(mProgramHandle, mMaterialSpecularHandle, &cMaterialSpecular[0]);
		renderer->SetUniformFloat3(mProgramHandle, mLightSpecularHandle, &cLightSpecular[0]);
		renderer->SetUniformFloat(mProgramHandle, mShininessFactorHandle, cShininessFactor);
		renderer->SetUniformFloat3(mProgramHandle, mLightPositionHandle, &cLightPosisiont[0]);

		renderer->SetIndexBuffer(mIndexesBufferHandle);
		renderer->DrawElements(PrimitiveType::Triangles, mObject.shapes[0].numIndices);

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

	ObjGeometry mObject;
	std::vector<uint8_t> mColors;

	VertexFormat mPosVertexFormat;
	ProgramHandle mProgramHandle;
	UniformHandle mModelViewProjHandle;
	UniformHandle mModelViewHandle;
	UniformHandle mMaterialAmbientHandle;
	UniformHandle mLightAmbientHandle;
	UniformHandle mMaterialDiffuseHandle;
	UniformHandle mLightDiffuseHandle;
	UniformHandle mMaterialSpecularHandle;
	UniformHandle mLightSpecularHandle;
	UniformHandle mShininessFactorHandle;
	UniformHandle mLightPositionHandle;

	VertexBufferHandle mPositionsHandle;
	VertexBufferHandle mNornalsHandle;
	VertexBufferHandle mColorsHandle;
	IndexBufferHandle mIndexesBufferHandle;

	TransformHelper mTransformHelper;

	glm::vec3 mSpeed{ 1.0f, 2.5f, 0.0f };
	glm::vec3 mAngles;
	glm::mat4 mProj;
	glm::mat4 mView;
	glm::vec3 mRight;
	glm::vec3 mUp;
};


extern "C" tinyngine::Application * CreateApplication() {
	return new PhongShading();
}