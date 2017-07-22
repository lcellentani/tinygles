#pragma once

#include <cstddef>
#include <cstdint>

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
