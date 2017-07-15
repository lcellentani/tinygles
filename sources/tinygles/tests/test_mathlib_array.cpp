#include "gtest/gtest.h"
#include "math/Array.h"

#include <algorithm>

using namespace mathlib;

TEST(MathlibArray, Construction) {
	const Array<float, 3> result;
	const float expected[]{ 0.0f, 0.0f, 0.0f };
	EXPECT_TRUE(std::equal(result.begin(), result.end(), expected));
}

TEST(MathlibArray, ConstructionArray) {
	const float expected[]{ 2.0f, -1.0f, 4.4f};
	const Array<float, 3> result(expected);
	EXPECT_TRUE(std::equal(result.begin(), result.end(), expected));
}

TEST(MathlibArray, Size) {
	const Array<float, 3> result;
	EXPECT_TRUE(result.Size() == 3);
}

TEST(MathlibArray, Fill) {
	Array<float, 4> result;
	result.fill(4.0f);

	const float expected[]{ 4.0f, 4.0f, 4.0f, 4.0f };
	EXPECT_TRUE(std::equal(result.begin(), result.end(), expected));
}

TEST(MathlibArray, Swap) {
	const float data[]{ 1.0f, 2.0f, 3.0f, 4.0f };
	const float swapped_data[]{ 4.0f, 3.0f, 2.0f, 1.0f };

	Array<float, 4> result(data);
	Array<float, 4> expected(swapped_data);

	result.swap(expected);

	EXPECT_TRUE(std::equal(result.begin(), result.end(), swapped_data));
	EXPECT_TRUE(std::equal(expected.begin(), expected.end(), data));
}

TEST(MathlibArray, Front) {
	const float data[]{ 2.0f, -1.0f, 4.4f, -7.0f };
	const Array<float, 4> mutable_result(data);
	Array<float, 3> const_result(data);

	EXPECT_FLOAT_EQ(mutable_result.front(), 2.0f);
	EXPECT_FLOAT_EQ(const_result.front(), 2.0f);
}

TEST(MathlibArray, Back) {
	const float data[]{ 2.0f, -1.0f, 4.4f, -7.0f };
	const Array<float, 4> mutable_result(data);
	Array<float, 3> const_result(data);

	EXPECT_FLOAT_EQ(mutable_result.back(), -7.0f);
	EXPECT_FLOAT_EQ(const_result.back(), 4.4f);
}

TEST(MathlibArray, Data) {
	const float data[]{ 1.0f, 2.0f, 3.0f, 5.0f };
	const Array<float, 4> const_result(data);
	for (std::size_t i = 0; i < const_result.Size(); i++) {
		EXPECT_FLOAT_EQ(const_result[i], data[i]);
	}

	Array<float, 3> mutable_result(data);
	for (std::size_t i = 0; i < mutable_result.Size(); i++) {
		EXPECT_FLOAT_EQ(mutable_result[i], data[i]);
	}
	const float* values = mutable_result.data();
	for (std::size_t i = 0; i < mutable_result.Size(); i++) {
		EXPECT_FLOAT_EQ(mutable_result[i], values[i]);
	}
}

TEST(MathlibArray, Iterator) {
	const float data[]{ 1.0f, 2.0f, 3.0f, 4.0f };

	const Array<float, 4> const_result(data);
	EXPECT_TRUE(std::equal(const_result.begin(), const_result.end(), data));

	Array<float, 4> mutable_result(data);
	EXPECT_TRUE(std::equal(mutable_result.begin(), mutable_result.end(), data));
}

TEST(MathlibArray, ReverseIterator) {
	const float data[]{ 1.0f, 2.0f, 3.0f, 4.0f };
	const float expected[]{ 4.0f, 3.0f, 2.0f, 1.0f };

	const Array<float, 4> const_result(data);
	EXPECT_TRUE(std::equal(const_result.rbegin(), const_result.rend(), expected));
	Array<float, 4> mutable_result(data);
	EXPECT_TRUE(std::equal(mutable_result.rbegin(), mutable_result.rend(), expected));
}