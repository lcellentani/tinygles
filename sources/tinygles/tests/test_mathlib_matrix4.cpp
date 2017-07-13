#include "gtest/gtest.h"

#include "math/Matrix4.h"

using namespace mathlib;

TEST(MathlibMatrix4, DataSizes) {
	const std::size_t result = mat4::Size;
	EXPECT_EQ(result, 16);

	const uint32_t rows = mat4::Rows;
	const uint32_t columns = mat4::Columns;
	EXPECT_EQ(rows, 4);
	EXPECT_EQ(columns, 4);
}

TEST(MathlibMatrix4, DefaultConstructor) {
	const mat4 result;
	const float expected[]{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	for (std::size_t n = 0; n < mat4::Size; n++) {
		EXPECT_FLOAT_EQ(result[n], expected[n]);
	}
}