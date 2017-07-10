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

private:
	Array<T, Size> mData;
};

typedef Vector3<int32_t> vec3i;
typedef Vector3<float> vec3;

} // namespace mathlib