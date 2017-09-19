#pragma once

#include <cstddef>
#include <cstdint>

#include <memory>

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>

#ifndef __has_feature
#define __has_feature(x) 0 // Compatibility with non-clang compilers.
#endif

namespace tinygles { namespace detail {
	template<typename T>
	void ignore(T&&) {}
}}
#define TINYGLES_UNUSED(x) tinygles::detail::ignore(x)


#if __cplusplus >= 201103L || _MSC_VER >= 1900 || __has_feature(cxx_constexpr)
namespace tinygles { namespace detail {
	// see http://reedbeta.com/blog/cpp-compile-time-array-size/
	template <typename T, int N> char(&countof(T(&)[N]))[N];
}}
#define TINYGLES_COUNTOF(arr) (sizeof(tinygles::detail::countof(arr)))
#elif _MSC_VER // Visual C++ fallback
#define TINYGLES_COUNTOF(arr) _countof(arr)
#else
TINYGLES_COUNTOF(arr) sizeof(arr) / sizeof(arr[0])
#endif

namespace tinygles
{

struct Color
{
	static constexpr Color Red() { return Color(255, 0, 0); }
	static constexpr Color Green() { return Color(0, 255, 0); }
	static constexpr Color Blue() { return Color(0, 0, 255); }
	static constexpr Color Black() { return Color(0, 0, 0); }
	static constexpr Color White() { return Color(255, 255, 255); }

	constexpr Color() : r(0), g(0), b(0), a(255) {}
	constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}

	constexpr const float red() const { return static_cast<float>(r / 256.0f); }
	constexpr const float green() const { return static_cast<float>(g / 256.0f); }
	constexpr const float blue() const { return static_cast<float>(b / 256.0f); }
	constexpr const float alpha() const { return static_cast<float>(a / 256.0f); }

	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

struct ResourceHandle {
	ResourceHandle() = default;
	ResourceHandle(uint32_t handle) : mHandle(handle) {}

	ResourceHandle& operator=(const ResourceHandle&) = default;

	inline const bool IsValid() const { return mHandle != 0; }

	uint32_t mHandle = 0;
};

using ShaderHandle = ResourceHandle;
using ProgramHandle = ResourceHandle;
using VertexBufferHandle = ResourceHandle;
using IndexBufferHandle = ResourceHandle;

} // namespace tinygles
