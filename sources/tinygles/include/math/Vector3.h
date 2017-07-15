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
class Vector3 {
public:
	using value_type = T;
	using value_type_ptr = T*;
	using const_value_type_ptr = const T*;
	using value_type_ref = T&;
	using const_value_type_ref = const T&;

public:
	static constexpr std::size_t Size = 3;

public:
	Vector3() {
		mData[0] = 0; mData[1] = 0; mData[2] = 0;
	}

	Vector3(const T& a0, const T& a1, const T& a2) {
		mData[0] = a0; mData[1] = a1; mData[2] = a2;
	}

	Vector3(const T& value) {
		mData[0] = value; mData[1] = value; mData[2] = value;
	}

	template<std::size_t N>
	Vector3(const T (&data)[N]) {
		assert(TINYGLES_COUNTOF(data) == Size);
		std::copy(data, data + Vector3::Size, mData.begin());
	}

	value_type_ptr data() {
		return mData.data();
	}

	const_value_type_ptr data() const {
		return mData.data();
	}

	value_type_ref operator[](const std::size_t n) {
		return mData[n];
	}

	const_value_type_ref operator[](const std::size_t n) const {
		return mData[n];
	}

	value_type_ref x() {
		return mData[0];
	}

	value_type_ref y() {
		return mData[1];
	}

	value_type_ref z() {
		return mData[2];
	}

	const_value_type_ref x() const {
		return mData[0];
	}

	const_value_type_ref y() const {
		return mData[1];
	}

	const_value_type_ref z() const {
		return mData[2];
	}

	static const Vector3 One() {
		return Vector3(static_cast<T>(1));
	}

	static const Vector3 Zero() {
		return Vector3(static_cast<T>(0));
	}

	static const Vector3 UnitX() {
		return Vector3(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));
	}

	static const Vector3 UnitY() {
		return Vector3(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
	}

	static const Vector3 UnitZ() {
		return Vector3(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
	}

	// Unary operators
	Vector3& operator-() {
		mData[0] = -mData[0];
		mData[1] = -mData[1];
		mData[2] = -mData[2];
		return *this;
	}

	const Vector3 operator-() const {
		return Vector3(-mData[0], -mData[1], -mData[2]);
	}

	// Addition operators
	Vector3& operator+=(const T& scalar) {
		mData[0] += scalar;
		mData[1] += scalar;
		mData[2] += scalar;
		return *this;
	}

	Vector3& operator+=(const Vector3& other) {
		mData[0] += other.mData[0];
		mData[1] += other.mData[1];
		mData[2] += other.mData[2];
		return *this;
	}

	const Vector3 operator+(const T& scalar) const {
		return Vector3(mData[0] + scalar, mData[1] + scalar, mData[2] + scalar);
	}

	const Vector3 operator+(const Vector3& other) const {
		return Vector3(mData[0] + other.mData[0], mData[1] + other.mData[1], mData[2] + other.mData[2]);
	}

	// Substraction operators
	Vector3& operator-=(const T& scalar) {
		mData[0] -= scalar;
		mData[1] -= scalar;
		mData[2] -= scalar;
		return *this;
	}

	Vector3& operator-=(const Vector3& other) {
		mData[0] -= other.mData[0];
		mData[1] -= other.mData[1];
		mData[2] -= other.mData[2];
		return *this;
	}

	const Vector3 operator-(const T& scalar) const {
		return Vector3(mData[0] - scalar, mData[1] - scalar, mData[2] - scalar);
	}

	const Vector3 operator-(const Vector3& other) const {
		return Vector3(mData[0] - other.mData[0], mData[1] - other.mData[1], mData[2] - other.mData[2]);
	}

	// Multiplication operators
	Vector3& operator*=(const T& scalar) {
		mData[0] *= scalar;
		mData[1] *= scalar;
		mData[2] *= scalar;
		return *this;
	}

	Vector3& operator*=(const Vector3& other) {
		mData[0] *= other.mData[0];
		mData[1] *= other.mData[1];
		mData[2] *= other.mData[2];
		return *this;
	}

	const Vector3 operator*(const T& scalar) const {
		return Vector3(mData[0] * scalar, mData[1] * scalar, mData[2] * scalar);
	}

	const Vector3 operator*(const Vector3& other) const {
		return Vector3(mData[0] * other.mData[0], mData[1] * other.mData[1], mData[2] * other.mData[2]);
	}

	// Division operators
	Vector3& operator/=(const T& scalar) {
		mData[0] /= scalar;
		mData[1] /= scalar;
		mData[2] /= scalar;
		return *this;
	}

	Vector3& operator/=(const Vector3& other) {
		mData[0] /= other.mData[0];
		mData[1] /= other.mData[1];
		mData[2] /= other.mData[2];
		return *this;
	}

	const Vector3 operator/(const T& scalar) const {
		return Vector3(mData[0] / scalar, mData[1] / scalar, mData[2] / scalar);
	}

	const Vector3 operator/(const Vector3& other) const {
		return Vector3(mData[0] / other.mData[0], mData[1] / other.mData[1], mData[2] / other.mData[2]);
	}

	// Comparison operators
	bool operator<(const Vector3& other) const {
		return mData[0] < other.mData[0] && mData[1] && other.mData[1] && mData[2] < other.mData[2];
	}

	bool operator> (const Vector3& other) const {
		return !(*this < other);
	}

	// Functions
	static const Vector3 min(const Vector3& a, const Vector3& b) {
		return Vector3(std::min(a.x(), b.x()), std::min(a.y(), b.y()), std::min(a.z(), b.z()));
	}

	static const Vector3 max(const Vector3& a, const Vector3& b) {
		return Vector3(std::max(a.x(), b.x()), std::max(a.y(), b.y()), std::max(a.z(), b.z()));
	}

	static T dot(const Vector3& a, const Vector3& b) {
		return (a.mData[0] * b.mData[0]) + (a.mData[1] * b.mData[1]) + (a.mData[2] * b.mData[2]);
	}

	template<typename U = T>
	static typename std::enable_if<std::is_floating_point<U>::value, const Vector3>::type cross(const Vector3& a, const Vector3& b) {
		const T rx = a.y() * b.z() - a.z() * b.y();
		const T ry = a.z() * b.x() - a.x() * b.z();
		const T rz = a.x() * b.y() - a.y() * b.x();
		return Vector3(rx, ry, rz);
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
	typename std::enable_if<std::is_floating_point<U>::value, Vector3&>::type normalize() {
		const T scalar = static_cast<T>(static_cast<T>(1) / magnitude());
		return *this *= scalar;
	}

	template<typename U = T>
	typename std::enable_if<std::is_floating_point<U>::value, Vector3>::type normalized() const {
		const T scalar = static_cast<T>(static_cast<T>(1) / magnitude());
		return Vector3(*this) *= scalar;
	}

private:
	Array<T, Size> mData;
};

template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
operator==(const Vector3<T>& v1, const Vector3<T>& v2) {
	const bool b1 = std::abs(v1.x() - v2.x()) < std::numeric_limits<T>::epsilon();
	const bool b2 = std::abs(v1.y() - v2.y()) < std::numeric_limits<T>::epsilon();
	const bool b3 = std::abs(v1.z() - v2.z()) < std::numeric_limits<T>::epsilon();
	return b1 && b2 && b3;
}

template<typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
operator==(const Vector3<T>& v1, const Vector3<T>& v2) {
	return v1.x() == v2.x() && v1.y() == v2.y() && v1.z() == v2.z();
}

template<typename T>
bool operator!=(const Vector3<T>& v1, const Vector3<T>& v2) {
	return !(v1 == v2);
}

typedef Vector3<int32_t> vec3i;
typedef Vector3<float> vec3;

} // namespace mathlib
