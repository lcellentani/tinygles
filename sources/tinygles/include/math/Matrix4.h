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
class Matrix4 {
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;

public:
	static constexpr std::size_t Size = 16;
	static constexpr uint32_t Rows = 4;
	static constexpr uint32_t Columns = 4;

public:
	Matrix4() {
		mData[0] = 1; mData[1] = 0; mData[2] = 0; mData[3] = 0;
		mData[4] = 0; mData[5] = 1; mData[6] = 0; mData[7] = 0;
		mData[8] = 0; mData[9] = 0; mData[10] = 1; mData[11] = 0;
		mData[12] = 0; mData[13] = 0; mData[14] = 0; mData[15] = 1;
	}

	Matrix4(const T* data) {
		std::copy(data, data + Matrix4::Size, mData.begin());
	}

	reference operator[](const std::size_t n) {
		return mData[n];
	}

	const_reference operator[](const std::size_t n) const {
		return mData[n];
	}

private:
	Array<T, Size> mData;
};

typedef Matrix4<int32_t> mat4i;
typedef Matrix4<float> mat4;

} // namespace mathlib