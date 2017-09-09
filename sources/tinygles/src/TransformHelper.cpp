#include "TransformHelper.h"

#include "PlatformDefine.h"

#include <string>

namespace tinygles
{

constexpr uint16_t cMaxModelMatrix = 8;
constexpr uint16_t cMaxViewMatrix = 8;
constexpr uint16_t cMaxProjectionMatrix = 2;
constexpr uint16_t cMaxTextureMatrix = 2;

struct TransformHelper::Impl {
	MatrixMode mMode;

	uint8_t mModelMatrixIndex;
	uint8_t mViewMatrixIndex;
	uint8_t mProjectionMatrixIndex;
	uint8_t mTextureMatrixIndex;

	glm::mat4 mModelMatrix[cMaxModelMatrix];
	glm::mat4 mViewMatrix[cMaxViewMatrix];
	glm::mat4 mProjectionMatrox[cMaxProjectionMatrix];
	glm::mat4 mTextureMatrix[cMaxTextureMatrix];

	glm::mat4 mMoldeViewMatrix;
	glm::mat4 mModelViewProjectionMatrix;
};

TransformHelper::TransformHelper() : mImpl(std::make_unique<Impl>()) {
	Reset();
}

TransformHelper::~TransformHelper() = default;

TransformHelper::TransformHelper(TransformHelper& rhs) : mImpl(nullptr) {
	if (rhs.mImpl != nullptr) {
		mImpl = std::make_unique<Impl>((*rhs.mImpl));
	}
}

TransformHelper& TransformHelper::operator=(TransformHelper& rhs) {
	if (rhs.mImpl == nullptr) {
		mImpl.reset();
	}
	else if (mImpl == nullptr) {
		mImpl = std::make_unique<Impl>((*rhs.mImpl));
	}
	else {
		*mImpl = *rhs.mImpl;
	}
	return (*this);
}

//====================================================================================================================

void TransformHelper::Reset() {
	mImpl->mMode = MatrixMode::None;
	mImpl->mModelMatrixIndex = 0;
	mImpl->mViewMatrixIndex = 0;
	mImpl->mProjectionMatrixIndex = 0;
	mImpl->mTextureMatrixIndex = 0;
}

void TransformHelper::SetMatrixMode(MatrixMode mode) {
	mImpl->mMode = mode;
}

void TransformHelper::LoadIdentity() {
	MatrixMode mode = mImpl->mMode;
	if (mode == MatrixMode::Model) {
		glm::mat4& m = GetModelMatrix();
		std::memset((void*)&m[0], 0, sizeof(glm::mat4));
		m[0][0] = 1.0f; m[1][1] = 1.0f; m[2][2] = 1.0;
		return;
	}
	if (mode == MatrixMode::View) {
		glm::mat4& m = GetViewMatrix();
		std::memset((void*)&m[0], 0, sizeof(glm::mat4));
		m[0][0] = 1.0f; m[1][1] = 1.0f; m[2][2] = 1.0;
		return;
	}
	if (mode == MatrixMode::Projection) {
		glm::mat4& m = GetProjectionMatrix();
		std::memset((void*)&m[0], 0, sizeof(glm::mat4));
		m[0][0] = 1.0f; m[1][1] = 1.0f; m[2][2] = 1.0;
		return;
	}
	if (mode == MatrixMode::Texture) {
		glm::mat4& m = GetTextureMatrix();
		std::memset((void*)&m[0], 0, sizeof(glm::mat4));
		m[0][0] = 1.0f; m[1][1] = 1.0f; m[2][2] = 1.0;
		return;
	}
}

void TransformHelper::LoadMatrix(glm::mat4& matrix) {
	TINYGLES_UNUSED(matrix);
}

void TransformHelper::MultiplyMatrix(glm::mat4& matrix) {
	TINYGLES_UNUSED(matrix);
}

void TransformHelper::Translate(glm::vec3& translation) {
	TINYGLES_UNUSED(translation);
}

void TransformHelper::Translate(float tx, float ty, float tz) {
	TINYGLES_UNUSED(tx); TINYGLES_UNUSED(ty); TINYGLES_UNUSED(tz);
}

void TransformHelper::Rotate(float angle_in_degree, glm::vec3& axis) {
	TINYGLES_UNUSED(angle_in_degree); TINYGLES_UNUSED(axis);
}

void TransformHelper::Rotate(float angle_in_degree, float ax, float ay, float az) {
	TINYGLES_UNUSED(angle_in_degree); TINYGLES_UNUSED(ax); TINYGLES_UNUSED(ay); TINYGLES_UNUSED(az);
}

void TransformHelper::Scale(glm::vec3& scale) {
	TINYGLES_UNUSED(scale);
}

void TransformHelper::Scale(float sx, float sy, float sz) {
	TINYGLES_UNUSED(sx); TINYGLES_UNUSED(sy); TINYGLES_UNUSED(sz);
}

void TransformHelper::PushMatrix() {

}

void TransformHelper::PopMatrix() {

}

glm::mat4& TransformHelper::GetModelMatrix() {
	return mImpl->mModelMatrix[mImpl->mModelMatrixIndex];
}

const glm::mat4& TransformHelper::GetModelMatrix() const {
	return mImpl->mModelMatrix[mImpl->mModelMatrixIndex];
}

glm::mat4& TransformHelper::GetViewMatrix() {
	return mImpl->mViewMatrix[mImpl->mViewMatrixIndex];
}

const glm::mat4& TransformHelper::GetViewMatrix() const {
	return mImpl->mViewMatrix[mImpl->mViewMatrixIndex];
}

glm::mat4& TransformHelper::GetProjectionMatrix() {
	return mImpl->mProjectionMatrox[mImpl->mProjectionMatrixIndex];
}

const glm::mat4& TransformHelper::GetProjectionMatrix() const {
	return mImpl->mProjectionMatrox[mImpl->mProjectionMatrixIndex];
}

glm::mat4& TransformHelper::GetTextureMatrix() {
	return mImpl->mTextureMatrix[mImpl->mTextureMatrixIndex];
}

const glm::mat4& TransformHelper::GetTextureMatrix() const {
	return mImpl->mTextureMatrix[mImpl->mTextureMatrixIndex];
}


} // namespace tinygles