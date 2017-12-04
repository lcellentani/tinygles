#include "ExampleBaseApp.h"
#include "GraphicsDevice.h"
#include "Time.h"
#include "MeshLoader.h"
#include "VertexFormat.h"
#include "TransformHelper.h"
#include "StringUtils.h"
#include "Log.h"
#include "imgui.h"

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace
{

static float cMaterialAmbient[] = { 0.2f, 0.0f, 0.2f };
static float cMaterialDiffuse[] = { 0.9f, 0.0f, 0.0f };
static float cMaterialSpecular[] = { 1.0f, 0.9f, 0.9f };
static float cShininessFactor = 15.0f;
static float cLightColor[] = { 1.0f, 1.0f, 1.0f };
static float cLightPosision[] = { 0.0f, 20.0f, 10.0f };

}

using namespace tinyngine;

class PhongShading final : public ExampleApp {
public:
	PhongShading() = default;
	virtual ~PhongShading() = default;

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

	void InitView(Engine& engine, uint32_t windowWidth, uint32_t windowHeight) override {
		ExampleApp::InitView(engine, windowWidth, windowHeight);

		GraphicsDevice& graphicsDevice = engine.GetSystem<GraphicsDevice>();
		MeshLoader& meshLoader = engine.GetSystem<MeshLoader>();

		//const char * filename = "models/Cube.obj";
		//const char * filename = "models/Sphere.obj";
		//const char * filename = "models/Teapot.obj";
		//const char * filename = "models/Cylinder.obj";
		const char * filename = "models/Monkey.obj";
		std::vector<MeshInfo> allMeshes = meshLoader.LoadObj(filename);
		if (allMeshes.size() == 0) {
			return;
		}

		auto& mesh = allMeshes[0];
		mNumIndices = mesh.mNumIndices;
		uint32_t numVertices = mesh.mNumVertices;
		mColors.reserve(numVertices * 4);
		for (uint32_t i = 0; i < numVertices * 4; i += 4) {
			mColors.push_back(255);
			mColors.push_back(255);
			mColors.push_back(255);
			mColors.push_back(255);
		}

		mPosVertexFormat.Add(Attributes::Position, AttributeType::Float, 3, false);
		mPosVertexFormat.Add(Attributes::Normal, AttributeType::Float, 3, false);
		mPosVertexFormat.Add(Attributes::Color0, AttributeType::Uint8, 4, true);

		mPositionsHandle = graphicsDevice.CreateVertexBuffer(&mesh.mPositions[0], sizeof(mesh.mPositions[0]) * numVertices * 3, mPosVertexFormat);
		mNornalsHandle = graphicsDevice.CreateVertexBuffer(&mesh.mNormals[0], sizeof(mesh.mNormals[0]) * numVertices * 3, mPosVertexFormat);
		mColorsHandle = graphicsDevice.CreateVertexBuffer(&mColors[0], sizeof(mColors[0]) * mColors.size() * 4, mPosVertexFormat);
		mIndexesBufferHandle = graphicsDevice.CreateIndexBuffer(&mesh.mIndices[0], sizeof(mesh.mIndices[0]) * mNumIndices);

		std::string vertexShaderSource;
		StringUtils::ReadFileToString("shaders/phong_vert_2.glsl", vertexShaderSource);
		std::string fragmentShaderSource;
		StringUtils::ReadFileToString("shaders/phong_frag_2.glsl", fragmentShaderSource);

		ShaderHandle vsHandle = graphicsDevice.CreateShader(ShaderType::VertexProgram, vertexShaderSource.c_str());
		ShaderHandle fsHandle = graphicsDevice.CreateShader(ShaderType::FragmentProgram, fragmentShaderSource.c_str());
		mProgramHandle = graphicsDevice.CreateProgram(vsHandle, fsHandle, true);
		mModelViewProjHandle = graphicsDevice.GetUniform(mProgramHandle, "u_modelViewProj");
		mModelViewHandle = graphicsDevice.GetUniform(mProgramHandle, "u_modelView");
		mMaterialAmbientHandle = graphicsDevice.GetUniform(mProgramHandle, "u_materialAmbient");
		mMaterialDiffuseHandle = graphicsDevice.GetUniform(mProgramHandle, "u_materialDiffuse");
		mMaterialSpecularHandle = graphicsDevice.GetUniform(mProgramHandle, "u_materialSpecular");
		mShininessFactorHandle = graphicsDevice.GetUniform(mProgramHandle, "u_shininessFactor");
		mLightColorHandle = graphicsDevice.GetUniform(mProgramHandle, "u_lightColor");
		mLightPositionHandle = graphicsDevice.GetUniform(mProgramHandle, "u_lightPosition");

		float ratio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
		mProj = glm::perspective(glm::radians(60.0f), ratio, 0.1f, 100.0f);
		mView = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		mUp = glm::vec3(0.0f, 1.0f, 0.0f);
		mRight = glm::vec3(1.0f, 0.0f, 0.0f);
		mForward = glm::vec3(0.0f, 0.0f, 1.0f);
		mAngles = glm::vec3(0.0f, 0.0f, 0.0f);

		mTransformHelper.SetMatrixMode(TransformHelper::MatrixMode::Projection);;
		mTransformHelper.LoadMatrix(mProj);
		mTransformHelper.SetMatrixMode(TransformHelper::MatrixMode::View);
		mTransformHelper.LoadMatrix(mView);

		graphicsDevice.SetState(RendererStateType::CullFace, true);
		graphicsDevice.SetState(RendererStateType::DepthTest, true);

		Time& time = engine.GetSystem<Time>();
		mLastTime = time.GetTime();
		clear_color = ImColor(114, 144, 154);

		mInitialized = true;
	}

	void RenderFrame(Engine& engine) override {
		ExampleApp::RenderFrame(engine);

		if (!mInitialized) { return; }

		GraphicsDevice& graphicsDevice = engine.GetSystem<GraphicsDevice>();
		Time& time = engine.GetSystem<Time>();

		int64_t now = time.GetTime();
		float delta = ((float)(now - mLastTime) / (float)time.GetFrequency()) * 1000.0f;
		mLastTime = now;

		char buffer[512];
		ImGui::SetNextWindowPos(ImVec2(5.0f, 5.0f), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Phong Shading", nullptr, ImVec2(200.0f, 200.0f), -1.f, ImGuiWindowFlags_AlwaysAutoResize);
		sprintf(buffer, "Time: %f", delta);
		ImGui::Text(buffer);
		ImGui::SliderFloat("RX", &mAngles.x, 0.0f, 360.0f, "%.2f");
		ImGui::SliderFloat("RY", &mAngles.y, 0.0f, 360.0f, "%.2f");
		ImGui::SliderFloat("RZ", &mAngles.z, 0.0f, 360.0f, "%.2f");
		ImGui::Spacing();
		ImGui::ColorEdit3("Light Color", (float*)&cLightColor);
		ImGui::InputFloat3("LightPos", cLightPosision, 2);
		ImGui::SliderFloat("Shininess", &cShininessFactor, 1.0f, 200.0f, "%.2f");
		ImGui::ColorEdit3("Material Ambient", (float*)&cMaterialAmbient);
		ImGui::ColorEdit3("Material Diffuse", (float*)&cMaterialDiffuse);
		ImGui::ColorEdit3("Material Specular", (float*)&cMaterialSpecular);
		ImGui::End();

		mTransformHelper.SetMatrixMode(TransformHelper::MatrixMode::Model);
		mTransformHelper.LoadIdentity();
		mTransformHelper.Rotate(mAngles.z, mForward);
		mTransformHelper.Rotate(mAngles.y, mUp);
		mTransformHelper.Rotate(mAngles.x, mRight);

		graphicsDevice.Clear(GraphicsDevice::ColorBuffer | GraphicsDevice::DepthBuffer, Color(80, 80, 80), 1.0f);

		graphicsDevice.SetVertexBuffer(mPositionsHandle, Attributes::Position);
		graphicsDevice.SetVertexBuffer(mNornalsHandle, Attributes::Normal);
		graphicsDevice.SetVertexBuffer(mColorsHandle, Attributes::Color0);

		graphicsDevice.SetProgram(mProgramHandle, mPosVertexFormat);
		graphicsDevice.SetUniformMat4(mProgramHandle, mModelViewProjHandle, &mTransformHelper.GetModelViewProjectionMatrix()[0][0], false);
		graphicsDevice.SetUniformMat4(mProgramHandle, mModelViewHandle, &mTransformHelper.GetModelViewMatrix()[0][0], false);
		graphicsDevice.SetUniformFloat3(mProgramHandle, mMaterialAmbientHandle, &cMaterialAmbient[0]);
		graphicsDevice.SetUniformFloat3(mProgramHandle, mMaterialDiffuseHandle, &cMaterialDiffuse[0]);
		graphicsDevice.SetUniformFloat3(mProgramHandle, mMaterialSpecularHandle, &cMaterialSpecular[0]);
		graphicsDevice.SetUniformFloat(mProgramHandle, mShininessFactorHandle, cShininessFactor);
		graphicsDevice.SetUniformFloat3(mProgramHandle, mLightColorHandle, &cLightColor[0]);
		graphicsDevice.SetUniformFloat3(mProgramHandle, mLightPositionHandle, &cLightPosision[0]);

		graphicsDevice.SetIndexBuffer(mIndexesBufferHandle);
		graphicsDevice.DrawElements(PrimitiveType::Triangles, mNumIndices);

		graphicsDevice.Commit();
	}

private:
	ImVec4 clear_color;

	bool mInitialized = false;
	uint32_t mNumIndices = 0;
	std::vector<uint8_t> mColors;
	int64_t mLastTime;

	VertexFormat mPosVertexFormat;
	ProgramHandle mProgramHandle;
	UniformHandle mModelViewProjHandle;
	UniformHandle mModelViewHandle;
	UniformHandle mMaterialAmbientHandle;
	UniformHandle mMaterialDiffuseHandle;
	UniformHandle mMaterialSpecularHandle;
	UniformHandle mShininessFactorHandle;
	UniformHandle mLightColorHandle;
	UniformHandle mLightPositionHandle;

	VertexBufferHandle mPositionsHandle;
	VertexBufferHandle mNornalsHandle;
	VertexBufferHandle mColorsHandle;
	IndexBufferHandle mIndexesBufferHandle;

	TransformHelper mTransformHelper;

	glm::vec3 mSpeed{ 0.05f, 0.06f, 0.0f };
	glm::vec3 mAngles;
	glm::mat4 mProj;
	glm::mat4 mView;
	glm::vec3 mRight;
	glm::vec3 mUp;
	glm::vec3 mForward;
};


extern "C" tinyngine::Application * CreateApplication() {
	return new PhongShading();
}