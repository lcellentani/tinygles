#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <limits>
#include <cmath>
#include <cassert>

#include "math/Array.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

namespace mathlib
{

template<typename T>
class Matrix4 {
public:
	using value_type = T;
	using value_type_ptr = T*;
	using const_value_type_ptr = const T*;
	using value_type_ref = T&;
	using const_value_type_ref = const T&;

public:
	static constexpr std::size_t Size = 16;
	static constexpr uint32_t Rows = 4;
	static constexpr uint32_t Columns = 4;

public:
	Matrix4() {
		mData[0] = 0; mData[1] = 0; mData[2] = 0; mData[3] = 0;
		mData[4] = 0; mData[5] = 0; mData[6] = 0; mData[7] = 0;
		mData[8] = 0; mData[9] = 0; mData[10] = 0; mData[11] = 0;
		mData[12] = 0; mData[13] = 0; mData[14] = 0; mData[15] = 0;
	}

	Matrix4(const_value_type_ptr data) {
		std::copy(data, data + Matrix4::Size, mData.begin());
	}

	Matrix4(const T& m00, const T& m01, const T& m02, const T& m03,
		const T& m10, const T& m11, const T& m12, const T& m13,
		const T& m20, const T& m21, const T& m22, const T& m23,
		const T& m30, const T& m31, const T& m32, const T& m33) {
		mData[0] = m00; mData[1] = m01; mData[2] = m02; mData[3] = m03;
		mData[4] = m10, mData[5] = m11; mData[6] = m12; mData[7] = m13;
		mData[8] = m20; mData[9] = m21; mData[10] = m22; mData[11] = m23;
		mData[12] = m30, mData[13] = m31; mData[14] = m32; mData[15] = m33;
	}

	value_type_ref operator[](const std::size_t n) {
		return mData[n];
	}

	const_value_type_ref operator[](const std::size_t n) const {
		return mData[n];
	}

	value_type_ref operator()(const std::size_t r, const std::size_t c) {
		return mData[(r * Rows) + c];
	}

	const_value_type_ref operator()(const std::size_t r, const std::size_t c) const {
		return mData[(r * Rows) + c];
	}

	// Data accessors
	value_type_ptr data() {
		return mData.data();
	}

	const_value_type_ptr data() const {
		return mData.data();
	}

	template<std::size_t N>
	void RowMajor(T(& data)[N]) const {
		assert(TINYGLES_COUNTOF(data) == Size);
		for (std::size_t r = 0; r < Rows; r++) {
			for (std::size_t c = 0; c < Columns; c++) {
				const std::size_t index = (r * Rows) + c;
				data[index] = mData[index];
			}
		}
	}

	template<std::size_t N>
	void ColumnMajor(T(& data)[N]) const {
		assert(TINYGLES_COUNTOF(data) == Size);
		for (std::size_t r = 0; r < Rows; r++) {
			for (std::size_t c = 0; c < Columns; c++) {
				const std::size_t src_index = (r * Rows) + c;
				const std::size_t dst_index = (c * Columns) + r;
				data[dst_index] = mData[src_index];
			}
		}
	}

	// Unary operators
	Matrix4& operator-() {
		mData[0] = -mData[0]; mData[1] = -mData[1]; mData[2] = -mData[2]; mData[3] = -mData[3];
		mData[4] = -mData[4]; mData[5] = -mData[5]; mData[6] = -mData[6]; mData[7] = -mData[7];
		mData[8] = -mData[8]; mData[9] = -mData[9]; mData[10] = -mData[10]; mData[11] = -mData[11];
		mData[12] = -mData[12]; mData[13] = -mData[13]; mData[14] = -mData[14]; mData[15] = -mData[15];
		return *this;
	}

	const Matrix4 operator-() const {
		return Matrix4(
			-mData[0], -mData[1], -mData[2], -mData[3],
			-mData[4], -mData[5], -mData[6], -mData[7],
			-mData[8], -mData[9], -mData[10], -mData[11],
			-mData[12], -mData[13], -mData[14], -mData[15]);
	}

	// Addition operators
	Matrix4& operator+=(const_value_type_ref scalar) {
		for (std::size_t n = 0; n < Size; n++) {
			mData[n] += scalar;
		}
		return *this;
	}

	Matrix4& operator+=(const Matrix4& other) {
		for (std::size_t n = 0; n < Size; n++) {
			mData[n] += other.mData[n];
		}
		return *this;
	}

	Matrix4 operator+(const_value_type_ref scalar) const {
		T data[Size];
		for (std::size_t n = 0; n < Size; n++) {
			data[n] = mData[n] + scalar;
		}
		return Matrix4(data);
	}

	Matrix4 operator+(const Matrix4& other) const {
		T data[Size];
		for (std::size_t n = 0; n < Size; n++) {
			data[n] = mData[n] + other.mData[n];
		}
		return Matrix4(data);
	}

	// Subtraction operators
	Matrix4& operator-=(const_value_type_ref scalar) {
		for (std::size_t n = 0; n < Size; n++) {
			mData[n] -= scalar;
		}
		return *this;
	}

	Matrix4& operator-=(const Matrix4& other) {
		for (std::size_t n = 0; n < Size; n++) {
			mData[n] -= other.mData[n];
		}
		return *this;
	}

	Matrix4 operator-(const_value_type_ref scalar) const {
		T data[Size];
		for (std::size_t n = 0; n < Size; n++) {
			data[n] = mData[n] - scalar;
		}
		return Matrix4(data);
	}

	Matrix4 operator-(const Matrix4& other) const {
		T data[Size];
		for (std::size_t n = 0; n < Size; n++) {
			data[n] = mData[n] - other.mData[n];
		}
		return Matrix4(data);
	}

	// Multiplication operators
	Matrix4& operator*=(const_value_type_ref scalar) {
		for (std::size_t n = 0; n < Size; n++) {
			mData[n] *= scalar;
		}
		return *this;
	}
	
	Matrix4& operator*=(const Matrix4& other) {
		Matrix4 copy_of_this(*this);
		for (std::size_t i = 0; i < Rows; i++) {
			for (std::size_t j = 0; j < Rows; j++) {
				T sum = 0;
				for (std::size_t k = 0; k < Rows; k++) {
					sum += copy_of_this(i, k) * other(k, j);
				}
				(*this)(i, j) = sum;
			}
		}
		return *this;
	}

	const Matrix4 operator*(const_value_type_ref scalar) const {
		T data[Size];
		for (std::size_t n = 0; n < Size; n++) {
			data[n] = mData[n] * scalar;
		}
		return Matrix4(data);
	}

	const Matrix4 operator*(const Matrix4& other) const {
		Matrix4 result;
		for (std::size_t i = 0; i < Rows; i++) {
			for (std::size_t j = 0; j < Rows; j++) {
				result(i, j) = 0;
				for (std::size_t k = 0; k < Rows; k++) {
					result(i, j) += (*this)(i, k) * other(k, j);
				}
			}
		}
		return result;
	}

	const Vector4<T> operator*(const Vector4<T>& other) const {
		Vector4<T> result;

		result.x() = other.x() * mData[ 0] + other.y() * mData[ 1] + other.z() * mData[ 2] + other.w() * mData[ 3];
		result.y() = other.x() * mData[ 4] + other.y() * mData[ 5] + other.z() * mData[ 6] + other.w() * mData[ 7];
		result.z() = other.x() * mData[ 8] + other.y() * mData[ 9] + other.z() * mData[10] + other.w() * mData[11];
		result.w() = other.x() * mData[12] + other.y() * mData[13] + other.z() * mData[14] + other.w() * mData[15];

		return result;
	}

	// Division operators
	Matrix4& operator/=(const_value_type_ref scalar) {
		for (std::size_t n = 0; n < Size; n++) {
			mData[n] /= scalar;
		}
		return *this;
	}

	const Matrix4 operator/(const_value_type_ref scalar) const {
		T data[Size];
		for (std::size_t n = 0; n < Size; n++) {
			data[n] = mData[n] / scalar;
		}
		return Matrix4(data);
	}

	// Functions
	static Matrix4 Identity() {
		return Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	}

	const Matrix4 Transpose() const {
		T data[Size];
		int k = 0;
		for (std::size_t r = 0; r < Rows; r++) {
			for (std::size_t c = 0; c < Columns; c++) {
				std::size_t idx = c * Columns + r;
				data[k++] = mData[idx];
			}
		}
		return Matrix4(data);
	}

	static const Matrix4 FromTranslationVector(const Vector3<T>& translation) {
		return Matrix4(
			1, 0, 0, translation.x(),
			0, 1, 0, translation.y(),
			0, 0, 1, translation.z(),
			0, 0, 0, 1
		);
	}

	static const Matrix4 FromTranslationVector(const_value_type_ref x, const_value_type_ref y, const_value_type_ref z) {
		return Matrix4(
			1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1
		);
	}

	static const Matrix4 FromScaleVector(const Vector3<T>& scale) {
		return Matrix4(
			scale.x(), 0, 0, 0,
			0, scale.y(), 0, 0,
			0, 0, scale.z(), 0,
			0, 0, 0, 1
		);
	}

	static const Matrix4 FromScaleVector(const_value_type_ref x, const_value_type_ref y, const_value_type_ref z) {
		return Matrix4(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1
		);
	}

	static const Matrix4 FromRotation(const_value_type_ref angle, const Vector3<T>& axis) {
		return Matrix4();
	}

	static const Matrix4 RotationX(const_value_type_ref angle) {
		return Matrix4();
	}

	static const Matrix4 RotationY(const_value_type_ref angle) {
		return Matrix4();
	}

	static const Matrix4 RotationZ(const_value_type_ref angle) {
		return Matrix4();
	}

	static const Matrix4 Perspective() {
		return Matrix4();
	}

	static const Matrix4 Ortho() {
		return Matrix4();
	}

	static const Matrix4 LookAt() {
		return Matrix4();
	}

	static const Matrix4 Unproject() {
		return Matrix4();
	}

private:
	Array<T, Size> mData;
};

template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
operator==(const Matrix4<T>& lhs, const Matrix4<T>& rhs) {
	bool result = true;
	for (int32_t i = static_cast<int32_t>(Matrix4<T>::Size - 1); i >= 0 && result; i--) {
		T a = lhs[i]; T b = rhs[i];
		T e = std::numeric_limits<T>::epsilon() * std::max(std::abs(a), std::abs(b));
		result &= (std::abs(a - b) <= 0.01f);
	}
	return result;
}

template<typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
operator==(const Matrix4<T>& lhs, const Matrix4<T>& rhs) {
	bool result = true;
	for (int32_t i = static_cast<int32_t>(Matrix4<T>::Size - 1); i >= 0 && result; i--) {
		result &= (lhs[i] == rhs[i]);
	}
	return result;
}

template<typename T>
bool operator!=(const Matrix4<T>& lhs, const Matrix4<T>& rhs) {
	return !(lhs == rhs);
}


typedef Matrix4<int32_t> mat4i;
typedef Matrix4<float> mat4;

} // namespace mathlib
