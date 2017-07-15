#pragma once

#include <memory>
#include <algorithm>

#include "PlatformDefine.h"

namespace mathlib
{

template<typename T, std::size_t Count>
class Array {
public:
	using value_type = T;
	using value_type_ptr = T*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator = T*;
	using const_iterator = const T*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
	Array() {
		std::fill(mData, mData + Count, static_cast<T>(0));
	}

	template<std::size_t N>
	Array(const T (&data)[N]) {
		assert(Count <= TINYGLES_COUNTOF(data));
		std::copy(data, data + Count, mData);
	}

	constexpr std::size_t Size() const {
		return Count;
	}

	void fill(const T& value) {
		std::fill_n(begin(), Count, value);
	}

	void swap(Array<T, Count>& other) {
		std::swap_ranges(begin(), end(), other.begin());
	}

	reference operator[](std::size_t n) {
		return mData[n];
	}

	const_reference operator[](std::size_t n) const {
		return mData[n];
	}

	iterator begin() {
		return iterator(std::addressof(mData[0]));
	}

	iterator end() {
		return iterator(std::addressof(mData[Count]));
	}

	const_iterator begin() const {
		return const_iterator(std::addressof(mData[0]));
	}

	const_iterator end() const {
		return const_iterator(std::addressof(mData[Count]));
	}

	reverse_iterator rbegin() {
		return reverse_iterator(end());
	}

	reverse_iterator rend() {
		return reverse_iterator(begin());
	}

	const_reverse_iterator rbegin() const {
		return const_reverse_iterator(end());
	}

	const_reverse_iterator rend() const {
		return const_reverse_iterator(begin());
	}

	reference front() {
		return *begin();
	}

	const_reference front() const {
		return *begin();
	}

	reference back() {
		return Count ? *(end() - 1) : *end();
	}

	const_reference back() const {
		return Count ? *(end() - 1) : *end();
	}

	value_type_ptr data() {
		return std::addressof(mData[0]);
	}

	const value_type_ptr data() const {
		return std::addressof(mData[0]);
	}

private:
	T mData[Count ? Count : 1];
};

} // namespace mathlib