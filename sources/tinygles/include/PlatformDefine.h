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


#if __cplusplus >= 201103L || _MSC_VER >= 1900 || __has_feature(cxx_constexpr) // C++ 11 implementation
namespace tinygles { namespace detail {
	template <typename T, std::size_t N>
	constexpr std::size_t countof(T const (&)[N]) noexcept {
		return N;
	}
}}
#define TINYGLES_COUNTOF(arr) tinygles::detail::countof(arr)
#elif _MSC_VER // Visual C++ fallback
#define TINYGLES_COUNTOF(arr) _countof(arr)
#elif __cplusplus >= 199711L && ( // C++ 98 trick
defined(__INTEL_COMPILER) || defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 4)))
	
template <typename T, std::size_t N>
char(&COUNTOF_REQUIRES_ARRAY_ARGUMENT(T(&)[N]))[N];

#define TINYGLES_COUNTOF(x) sizeof(COUNTOF_REQUIRES_ARRAY_ARGUMENT(x))
#else
TINYGLES_COUNTOF(arr) sizeof(arr) / sizeof(arr[0])
#endif
