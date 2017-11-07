#include "Application.h"
#include "GraphicsDevice.h"
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

class GouraudShading : public Application {
public:
	GouraudShading() = default;
	virtual ~GouraudShading() {

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

	void InitView(Engine& engine, uint32_t windowWidth, uint32_t windowHeight) override {
		GraphicsDevice& graphicsDevice = engine.GetSystem<GraphicsDevice>();
		//LoadObj("models/Cube.obj", true, mObject);
		//LoadObj("models/Sphere.obj", true, mObject);
		LoadObj("models/Monkey.obj", true, mObject);

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

		mPositionsHandle = graphicsDevice.CreateVertexBuffer(&shape.positions[0], sizeof(shape.positions[0]) * shape.numVertices * 3, mPosVertexFormat);
		mNornalsHandle = graphicsDevice.CreateVertexBuffer(&shape.normals[0], sizeof(shape.normals[0]) * shape.numVertices * 3, mPosVertexFormat);
		mColorsHandle = graphicsDevice.CreateVertexBuffer(&mColors[0], sizeof(mColors[0]) * mColors.size() * 4, mPosVertexFormat);
		mIndexesBufferHandle = graphicsDevice.CreateIndexBuffer(&shape.indices[0], sizeof(shape.indices[0]) * shape.numIndices);

		std::string vertexShaderSource;
		StringUtils::ReadFileToString("shaders/gouraud_vert_2.glsl", vertexShaderSource);
		std::string fragmentShaderSource;
		StringUtils::ReadFileToString("shaders/gouraud_frag_2.glsl", fragmentShaderSource);

		ShaderHandle vsHandle = graphicsDevice.CreateShader(ShaderType::VertexProgram, vertexShaderSource.c_str());
		ShaderHandle fsHandle = graphicsDevice.CreateShader(ShaderType::FragmentProgram, fragmentShaderSource.c_str());
		mProgramHandle = graphicsDevice.CreateProgram(vsHandle, fsHandle, true);
		mModelViewProjHandle = graphicsDevice.GetUniform(mProgramHandle, "u_modelViewProj");
		mModelViewHandle = graphicsDevice.GetUniform(mProgramHandle, "u_modelView");
		mMaterialAmbientHandle = graphicsDevice.GetUniform(mProgramHandle, "u_materialAmbient");
		mLightAmbientHandle = graphicsDevice.GetUniform(mProgramHandle, "u_lightAmbient");
		mMaterialDiffuseHandle = graphicsDevice.GetUniform(mProgramHandle, "u_materialDiffuse");
		mLightDiffuseHandle = graphicsDevice.GetUniform(mProgramHandle, "u_lightDiffuse");
		mMaterialSpecularHandle = graphicsDevice.GetUniform(mProgramHandle, "u_materialSpecular");
		mLightSpecularHandle = graphicsDevice.GetUniform(mProgramHandle, "u_lightSpecular");
		mShininessFactorHandle = graphicsDevice.GetUniform(mProgramHandle, "u_shininessFactor");
		mLightPositionHandle = graphicsDevice.GetUniform(mProgramHandle, "u_lightPosition");

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
		graphicsDevice.SetState(RendererStateType::DepthTest, true);
	}

	void RenderFrame(Engine& engine) override {
		GraphicsDevice& graphicsDevice = engine.GetSystem<GraphicsDevice>();
		mAngles.x += mSpeed.x;
		if (mAngles.x > 360.0f) {
			mAngles.x -= 360.0f;
		}
		mAngles.y += mSpeed.y;
		if (mAngles.y > 360.0f) {
			mAngles.y -= 360.0f;
		}
		
		mTransformHelper.SetMatrixMode(TransformHelper::MatrixMode::Model);
		mTransformHelper.LoadIdentity();
		mTransformHelper.Rotate(mAngles.y, mUp);
		mTransformHelper.Rotate(-mAngles.x, mRight);

		graphicsDevice.Clear(GraphicsDevice::ColorBuffer | GraphicsDevice::DepthBuffer, Color(92, 92, 92), 1.0f);

		graphicsDevice.SetVertexBuffer(mPositionsHandle, Attributes::Position);
		graphicsDevice.SetVertexBuffer(mNornalsHandle, Attributes::Normal);
		graphicsDevice.SetVertexBuffer(mColorsHandle, Attributes::Color0);

		graphicsDevice.SetProgram(mProgramHandle, mPosVertexFormat);
		graphicsDevice.SetUniformMat4(mProgramHandle, mModelViewProjHandle, &mTransformHelper.GetModelViewProjectionMatrix()[0][0], false);
		graphicsDevice.SetUniformMat4(mProgramHandle, mModelViewHandle, &mTransformHelper.GetModelViewMatrix()[0][0], false);
		graphicsDevice.SetUniformFloat3(mProgramHandle, mMaterialAmbientHandle, &cMaterialAmbient[0]);
		graphicsDevice.SetUniformFloat3(mProgramHandle, mLightAmbientHandle, &cLightAmbient[0]);
		graphicsDevice.SetUniformFloat3(mProgramHandle, mMaterialDiffuseHandle, &cMaterialDiffuse[0]);
		graphicsDevice.SetUniformFloat3(mProgramHandle, mLightDiffuseHandle, &cLightDiffuse[0]);
		graphicsDevice.SetUniformFloat3(mProgramHandle, mMaterialSpecularHandle, &cMaterialSpecular[0]);
		graphicsDevice.SetUniformFloat3(mProgramHandle, mLightSpecularHandle, &cLightSpecular[0]);
		graphicsDevice.SetUniformFloat(mProgramHandle, mShininessFactorHandle, cShininessFactor);
		graphicsDevice.SetUniformFloat3(mProgramHandle, mLightPositionHandle, &cLightPosisiont[0]);

		graphicsDevice.SetIndexBuffer(mIndexesBufferHandle);
		graphicsDevice.DrawElements(PrimitiveType::Triangles, mObject.shapes[0].numIndices);

		graphicsDevice.Commit();
	}

	void ReleaseView(Engine&) override {
	}

	void ReleaseApplication() override {

	}

private:
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

	glm::vec3 mSpeed{ 2.f, 1.f, 0.0f };
	glm::vec3 mAngles;
	glm::mat4 mProj;
	glm::mat4 mView;
	glm::vec3 mRight;
	glm::vec3 mUp;
};


extern "C" tinyngine::Application * CreateApplication() {
	return new GouraudShading();
}