#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <limits>
#include <cmath>

#include "math/Array.h"

namespace mathlib
{

template<typename T>
class Vector4 {
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;

public:
	static constexpr std::size_t Size = 4;

public:
	Vector4() {
		mData[0] = 0; mData[1] = 0; mData[2] = 0; mData[3] = 0;
	}

	Vector4(const T& a0, const T& a1, const T& a2, const T& a3) {
		mData[0] = a0; mData[1] = a1; mData[2] = a2; mData[3] = a3;
	}

	Vector4(const T& value) {
		mData[0] = value; mData[1] = value; mData[2] = value; mData[3] = value;
	}

	Vector4(const T* data) {
		std::copy(data, data + Vector4::Size, mData.begin());
	}

	const_pointer data() const {
		return mData.data();
	}

	reference operator[](const std::size_t n) {
		return mData[n];
	}

	const_reference operator[](const std::size_t n) const {
		return mData[n];
	}

	reference x() {
		return mData[0];
	}

	reference y() {
		return mData[1];
	}

	reference z() {
		return mData[2];
	}

	reference w() {
		return mData[3];
	}

	const_reference x() const {
		return mData[0];
	}

	const_reference y() const {
		return mData[1];
	}

	const_reference z() const {
		return mData[2];
	}

	const_reference w() const {
		return mData[3];
	}

	static const Vector4 One() {
		return Vector4(static_cast<T>(1));
	}

	static const Vector4 Zero() {
		return Vector4(static_cast<T>(0));
	}

	static const Vector4 UnitX() {
		return Vector4(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0));
	}

	static const Vector4 UnitY() {
		return Vector4(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));
	}

	static const Vector4 UnitZ() {
		return Vector4(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
	}
	static const Vector4 UnitW() {
		return Vector4(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
	}

	// Unary operators
	Vector4& operator-() {
		mData[0] = -mData[0];
		mData[1] = -mData[1];
		mData[2] = -mData[2];
		mData[3] = -mData[3];
		return *this;
	}

	const Vector4 operator-() const {
		return Vector4(-mData[0], -mData[1], -mData[2], -mData[3]);
	}

	// Addition operators
	Vector4& operator+=(const T& scalar) {
		mData[0] += scalar;
		mData[1] += scalar;
		mData[2] += scalar;
		mData[3] += scalar;
		return *this;
	}

	Vector4& operator+=(const Vector4& other) {
		mData[0] += other.mData[0];
		mData[1] += other.mData[1];
		mData[2] += other.mData[2];
		mData[3] += other.mData[3];
		return *this;
	}

	const Vector4 operator+(const T& scalar) const {
		return Vector4(mData[0] + scalar, mData[1] + scalar, mData[2] + scalar, mData[3] + scalar);
	}

	const Vector4 operator+(const Vector4& other) const {
		return Vector4(
			mData[0] + other.mData[0],
			mData[1] + other.mData[1],
			mData[2] + other.mData[2],
			mData[3] + other.mData[3]
		);
	}

	// Substraction operators
	Vector4& operator-=(const T& scalar) {
		mData[0] -= scalar;
		mData[1] -= scalar;
		mData[2] -= scalar;
		mData[3] -= scalar;
		return *this;
	}

	Vector4& operator-=(const Vector4& other) {
		mData[0] -= other.mData[0];
		mData[1] -= other.mData[1];
		mData[2] -= other.mData[2];
		mData[3] -= other.mData[3];
		return *this;
	}

	const Vector4 operator-(const T& scalar) const {
		return Vector4(mData[0] - scalar, mData[1] - scalar, mData[2] - scalar, mData[3] - scalar);
	}

	const Vector4 operator-(const Vector4& other) const {
		return Vector4(
			mData[0] - other.mData[0],
			mData[1] - other.mData[1],
			mData[2] - other.mData[2],
			mData[3] - other.mData[3]
		);
	}

	// Multiplication operators
	Vector4& operator*=(const T& scalar) {
		mData[0] *= scalar;
		mData[1] *= scalar;
		mData[2] *= scalar;
		mData[3] *= scalar;
		return *this;
	}

	Vector4& operator*=(const Vector4& other) {
		mData[0] *= other.mData[0];
		mData[1] *= other.mData[1];
		mData[2] *= other.mData[2];
		mData[3] *= other.mData[3];
		return *this;
	}

	const Vector4 operator*(const T& scalar) const {
		return Vector4(mData[0] * scalar, mData[1] * scalar, mData[2] * scalar, mData[3] * scalar);
	}

	const Vector4 operator*(const Vector4& other) const {
		return Vector4(
			mData[0] * other.mData[0],
			mData[1] * other.mData[1],
			mData[2] * other.mData[2],
			mData[3] * other.mData[3]
		);
	}

	// Division operators
	Vector4& operator/=(const T& scalar) {
		mData[0] /= scalar;
		mData[1] /= scalar;
		mData[2] /= scalar;
		mData[3] /= scalar;
		return *this;
	}

	Vector4& operator/=(const Vector4& other) {
		mData[0] /= other.mData[0];
		mData[1] /= other.mData[1];
		mData[2] /= other.mData[2];
		mData[3] /= other.mData[3];
		return *this;
	}

	const Vector4 operator/(const T& scalar) const {
		return Vector4(mData[0] / scalar, mData[1] / scalar, mData[2] / scalar, mData[3] / scalar);
	}

	const Vector4 operator/(const Vector4& other) const {
		return Vector4(
			mData[0] / other.mData[0],
			mData[1] / other.mData[1],
			mData[2] / other.mData[2],
			mData[3] / other.mData[3]
		);
	}

	// Comparison operators
	bool operator<(const Vector4& other) const {
		return mData[0] < other.mData[0] && mData[1] && other.mData[1] && mData[2] < other.mData[2] && mData[3] < other.mData[3];
	}

	bool operator> (const Vector4& other) const {
		return !(*this < other);
	}

	// Functions
	static const Vector4 min(const Vector4& a, const Vector4& b) {
		return Vector4(std::min(a.x(), b.x()), std::min(a.y(), b.y()), std::min(a.z(), b.z()), std::min(a.w(), b.w()));
	}

	static const Vector4 max(const Vector4& a, const Vector4& b) {
		return Vector4(std::max(a.x(), b.x()), std::max(a.y(), b.y()), std::max(a.z(), b.z()), std::max(a.w(), b.w()));
	}

	static T dot(const Vector4& a, const Vector4& b) {
		return (a.mData[0] * b.mData[0]) + (a.mData[1] * b.mData[1]) + (a.mData[2] * b.mData[2]) + (a.mData[3] * b.mData[3]);
	}

	template<typename U = T>
	typename std::enable_if<std::is_floating_point<U>::value, T>::type magnitude() const {
		T sum = dot(*this, *this);
		if (std::abs(sum - 1.0f) > std::numeric_limits<T>::epsilon()) {
			return std::sqrt(sum);
		}
		return 1.0f;
	}

	template<typename U = T>
	typename std::enable_if<std::is_floating_point<U>::value, Vector4&>::type normalize() {
		const T scalar = static_cast<T>(static_cast<T>(1) / magnitude());
		return *this *= scalar;
	}

	template<typename U = T>
	typename std::enable_if<std::is_floating_point<U>::value, Vector4>::type normalized() const {
		const T scalar = static_cast<T>(static_cast<T>(1) / magnitude());
		return Vector4(*this) *= scalar;
	}

private:
	Array<T, Size> mData;
};

template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
operator==(const Vector4<T>& v1, const Vector4<T>& v2) {
	const bool b1 = std::abs(v1.x() - v2.x()) < std::numeric_limits<T>::epsilon();
	const bool b2 = std::abs(v1.y() - v2.y()) < std::numeric_limits<T>::epsilon();
	const bool b3 = std::abs(v1.z() - v2.z()) < std::numeric_limits<T>::epsilon();
	const bool b4 = std::abs(v1.w() - v2.w()) < std::numeric_limits<T>::epsilon();
	return b1 && b2 && b3 && b4;
}

template<typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
operator==(const Vector4<T>& v1, const Vector4<T>& v2) {
	return v1.x() == v2.x() && v1.y() == v2.y() && v1.z() == v2.z() && v1.w() == v2.w();
}

template<typename T>
bool operator!=(const Vector4<T>& v1, const Vector4<T>& v2) {
	return !(v1 == v2);
}

typedef Vector4<int32_t> vec4i;
typedef Vector4<float> vec4;

} // namespace mathlib
