#include "ExampleBaseApp.h"
#include "GraphicsDevice.h"
#include "ImageManager.h"
#include "MeshLoader.h"
#include "VertexFormat.h"
#include "TransformHelper.h"
#include "StringUtils.h"
#include "Log.h"

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

using namespace tinyngine;

class MultiTexture final : public ExampleApp {
public:
	MultiTexture() = default;
	virtual ~MultiTexture() = default;

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
		std::vector<MeshInfo> allMeshes = meshLoader.LoadObj("models/Cube.obj");
		if (allMeshes.size() == 0) {
			return;
		}
		auto& mesh = allMeshes[0];
		uint32_t numVertices = mesh.mNumVertices;
		mNumIndices = mesh.mNumIndices;

		mColors.reserve(numVertices * 4);
		for (uint32_t i = 0; i < numVertices * 4; i += 4) {
			mColors.push_back(255);
			mColors.push_back(255);
			mColors.push_back(255);
			mColors.push_back(255);
		}

		mPosVertexFormat.Add(Attributes::Position, AttributeType::Float, 3, false);
		mPosVertexFormat.Add(Attributes::TexCoord0, AttributeType::Float, 2, false);
		mPosVertexFormat.Add(Attributes::Normal, AttributeType::Float, 3, false);
		mPosVertexFormat.Add(Attributes::Color0, AttributeType::Uint8, 4, true);

		mPositionsHandle = graphicsDevice.CreateVertexBuffer(&mesh.mPositions[0], sizeof(mesh.mPositions[0]) * numVertices * 3, mPosVertexFormat);
		mTexcoords0Handle = graphicsDevice.CreateVertexBuffer(&mesh.mTexcoords[0], sizeof(mesh.mTexcoords[0]) * numVertices * 2, mPosVertexFormat);
		mNornalsHandle = graphicsDevice.CreateVertexBuffer(&mesh.mNormals[0], sizeof(mesh.mNormals[0]) * numVertices * 3, mPosVertexFormat);
		mColorsHandle = graphicsDevice.CreateVertexBuffer(&mColors[0], sizeof(mColors[0]) * mColors.size() * 4, mPosVertexFormat);
		mIndexesBufferHandle = graphicsDevice.CreateIndexBuffer(&mesh.mIndices[0], sizeof(mesh.mIndices[0]) * mNumIndices);

		std::string vertexShaderSource;
		StringUtils::ReadFileToString("shaders/multitexture_vert_2.glsl", vertexShaderSource);
		std::string fragmentShaderSource;
		StringUtils::ReadFileToString("shaders/multitexture_frag_2.glsl", fragmentShaderSource);

		ShaderHandle vsHandle = graphicsDevice.CreateShader(ShaderType::VertexProgram, vertexShaderSource.c_str());
		ShaderHandle fsHandle = graphicsDevice.CreateShader(ShaderType::FragmentProgram, fragmentShaderSource.c_str());
		mProgramHandle = graphicsDevice.CreateProgram(vsHandle, fsHandle, true);
		mModelViewProjHandle = graphicsDevice.GetUniform(mProgramHandle, "u_modelViewProj");
		mModelViewHandle = graphicsDevice.GetUniform(mProgramHandle, "u_modelView");
		mLightPositionHandle = graphicsDevice.GetUniform(mProgramHandle, "u_lightPosition");
		mTexture0Handle = graphicsDevice.GetUniform(mProgramHandle, "u_texture0");
		mTexture1Handle = graphicsDevice.GetUniform(mProgramHandle, "u_texture1");

		ImageManager& imageManager = engine.GetSystem<ImageManager>();

		ImageHandle handle1 = imageManager.LoadImageFromFile("textures/woodenbox.png");
		mPrimaryTextureHandle = graphicsDevice.CreateTexture2D(handle1, imageManager, TextureFormats::RGB8, TextureFilteringMode::Trilinear, true);
		imageManager.ReleaseImage(handle1);

		ImageHandle handle2 = imageManager.LoadImageFromFile("textures/smile.png");
		mSecondaryTextureHandle = graphicsDevice.CreateTexture2D(handle2, imageManager, TextureFormats::RGBA8, TextureFilteringMode::Trilinear, true);
		imageManager.ReleaseImage(handle2);

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


		mInitialized = true;
	}

	void RenderFrame(Engine& engine) override {
		ExampleApp::RenderFrame(engine);

		if (!mInitialized) { return; }

		static float cLightPosisiont[] = { 0.0f, 00.0f, 10.0f };

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
		graphicsDevice.SetVertexBuffer(mTexcoords0Handle, Attributes::TexCoord0);
		graphicsDevice.SetVertexBuffer(mColorsHandle, Attributes::Color0);

		graphicsDevice.SetTexture(0, mPrimaryTextureHandle);
		graphicsDevice.SetTexture(1, mSecondaryTextureHandle);

		graphicsDevice.SetProgram(mProgramHandle, mPosVertexFormat);
		graphicsDevice.SetUniformMat4(mProgramHandle, mModelViewProjHandle, &mTransformHelper.GetModelViewProjectionMatrix()[0][0], false);
		graphicsDevice.SetUniformMat4(mProgramHandle, mModelViewHandle, &mTransformHelper.GetModelViewMatrix()[0][0], false);
		graphicsDevice.SetUniformFloat3(mProgramHandle, mLightPositionHandle, &cLightPosisiont[0]);
		graphicsDevice.setUniform1i(mProgramHandle, mTexture0Handle, 0);
		graphicsDevice.setUniform1i(mProgramHandle, mTexture1Handle, 1);

		graphicsDevice.SetIndexBuffer(mIndexesBufferHandle);
		graphicsDevice.DrawElements(PrimitiveType::Triangles, mNumIndices);

		graphicsDevice.Commit();
	}

private:
	VertexFormat mPosVertexFormat;
	ProgramHandle mProgramHandle;

	VertexBufferHandle mPositionsHandle;
	VertexBufferHandle mTexcoords0Handle;
	VertexBufferHandle mNornalsHandle;
	VertexBufferHandle mColorsHandle;
	IndexBufferHandle mIndexesBufferHandle;
	TextureHandle mPrimaryTextureHandle;
	TextureHandle mSecondaryTextureHandle;

	UniformHandle mModelViewProjHandle;
	UniformHandle mModelViewHandle;
	UniformHandle mLightPositionHandle;
	UniformHandle mTexture0Handle;
	UniformHandle mTexture1Handle;

	bool mInitialized = false;
	uint32_t mNumIndices = 0;
	std::vector<uint8_t> mColors;

	TransformHelper mTransformHelper;

	glm::vec3 mSpeed{ 2.f, 1.f, 0.0f };
	glm::vec3 mAngles;
	glm::mat4 mProj;
	glm::mat4 mView;
	glm::vec3 mRight;
	glm::vec3 mUp;
};


extern "C" tinyngine::Application * CreateApplication() {
	return new MultiTexture();
}