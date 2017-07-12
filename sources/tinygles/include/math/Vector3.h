#pragma once

#include <cstddef>
#include <cstdint>

#include "math/Array.h"


namespace mathlib
{

template<typename T>
class Vector3 {
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;

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

	Vector3(const T* other) {
		std::copy(other, other + Vector3::Size, mData.begin());
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

	const_reference x() const {
		return mData[0];
	}

	const_reference y() const {
		return mData[1];
	}

	const_reference z() const {
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

private:
	Array<T, Size> mData;
};

typedef Vector3<int32_t> vec3i;
typedef Vector3<float> vec3;

} // namespace mathlib