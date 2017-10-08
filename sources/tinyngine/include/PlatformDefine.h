#pragma once

#include <cstddef>
#include <cstdint>

#include <memory>
#include <atomic>
#include <thread>

#ifndef __has_feature
#define __has_feature(x) 0 // Compatibility with non-clang compilers.
#endif

namespace tinyngine { namespace detail {
	template<typename T>
	void ignore(T&&) {}
}}
#define TINYNGINE_UNUSED(x) tinyngine::detail::ignore(x)


#if __cplusplus >= 201103L || _MSC_VER >= 1900 || __has_feature(cxx_constexpr)
namespace tinyngine { namespace detail {
	// see http://reedbeta.com/blog/cpp-compile-time-array-size/
	template <typename T, int N> char(&countof(T(&)[N]))[N];
}}
#define TINYNGINE_COUNTOF(arr) (sizeof(tinyngine::detail::countof(arr)))
#elif _MSC_VER // Visual C++ fallback
#define TINYNGINE_COUNTOF(arr) _countof(arr)
#else
#define TINYNGINE_COUNTOF(arr) sizeof(arr) / sizeof(arr[0])
#endif

