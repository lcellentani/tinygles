#pragma once

#include "PlatformDefine.h"

#include <array>

namespace tinyngine
{

template<typename T, size_t Size>
class SpScLockFreeQueue {
public:
	SpScLockFreeQueue() {
		mRead.store(0);
		mWrite.store(0);
	}

	~SpScLockFreeQueue() {

	}

	bool tryPush(const T& elem) {
		const auto currentTail = mWrite.load();
		if (currentTail - mRead.load() < mSize - 1) {
			mBuffer[currentTail % mSize] = elem;
			mWrite.store(currentTail + 1);
			return true;
		}
		return false;
	}

	void push(const T& elem) {
		while (!tryPush(elem));
	}

	bool tryPop(T* elem) {
		const auto currentHead = mRead.load();
		if (currentHead != mWrite.load()) {
			(*elem) = mBuffer[currentHead & mSize];
			mRead.store(currentHead + 1);
			return true;
		}
		return false;
	}

	T pop() {
		T elem;
		while (!tryPop(&elem));
		return elem;
	}

	SpScLockFreeQueue(const SpScLockFreeQueue& other) = delete;
	const SpScLockFreeQueue& operator=(const SpScLockFreeQueue& other) = delete;
	
	SpScLockFreeQueue(const SpScLockFreeQueue&& other) = delete;
	SpScLockFreeQueue& operator=(const SpScLockFreeQueue&& other) = delete;

private:
	static constexpr size_t BufferSize = Size + 1;
	uint64_t mSize = BufferSize;
	std::array<T, BufferSize> mBuffer;
	std::atomic<size_t> mRead;
	std::atomic<size_t> mWrite;
};

} // namespace tinyngine