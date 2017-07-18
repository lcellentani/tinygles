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
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};

	for (std::size_t n = 0; n < mat4::Size; n++) {
		EXPECT_FLOAT_EQ(result[n], expected[n]);
	}
}

TEST(MathlibMatrix4, ArrayConstructor) {
	const float data[]{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	const mat4 result(data);
	const mat4 expected(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	for (std::size_t n = 0; n < mat4::Size; n++) {
		EXPECT_FLOAT_EQ(result[n], expected[n]);
	}
}

TEST(MathlibMatrix4, ScalarConstructor) {
	const mat4 result(
		1.0f, 0.0f, 0.0f, -2.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		7.5f, 4.0f, 3.0f, 1.0f
	);
	const mat4 expected(
		1.0f, 0.0f, 0.0f, -2.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		7.5f, 4.0f, 3.0f, 1.0f
	);

	for (std::size_t n = 0; n < mat4::Size; n++) {
		EXPECT_FLOAT_EQ(result[n], expected[n]);
	}
}

template <typename T, std::size_t N>
constexpr std::size_t countof(T const (&)[N]) noexcept
{
	return N;
}

TEST(MathlibMatrix4, RowMajorConstructor) {
	const float data[]{
		1.0f, 0.0f, 0.0f, -2.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		7.5f, 4.0f, 3.0f, 1.0f
	};
	const mat4 expected(
		1.0f, 0.0f, 0.0f, -2.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		7.5f, 4.0f, 3.0f, 1.0f
	);

	mat4 mat(data);
	float result[16];
	mat.RowMajor(result);

	for (std::size_t n = 0; n < mat4::Size; n++) {
		EXPECT_FLOAT_EQ(result[n], expected[n]);
	}
}

TEST(MathlibMatrix4, ColumnMajorConstructor) {
	const float data[]{
		1.0f, 0.0f, 0.0f, -2.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		7.5f, 4.0f, 3.0f, 1.0f
	};
	const mat4 expected(
		1.0f, 0.0f, 0.0f, 7.5f,
		0.0f, 1.0f, 0.0f, 4.0f,
		0.0f, 0.0f, 1.0f, 3.0f,
		-2.0f, 0.0f, 0.0f, 1.0f
	);

	mat4 mat(data);
	float result[16];
	mat.ColumnMajor(result);

	for (std::size_t n = 0; n < mat4::Size; n++) {
		EXPECT_FLOAT_EQ(result[n], expected[n]);
	}
}

TEST(MathlibMatrix4, OperationUnary) {
	const mat4 A(
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 2.0f, 3.0f, 1.0f
	);
	mat4 B(
		-1.0f, 1.0f, 2.0f, 3.0f,
		0.0f, -1.0f, 0.0f, 1.0f,
		2.0f, 0.0f, 1.0f, 0.0f,
		0.0f, -1.0f, -3.0f, 1.0f
	);

	const mat4 expected1(
		-1.0f, 0.0f, 0.0f, -1.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		-1.0f, -2.0f, -3.0f, -1.0f
	);
	const mat4 expected2(
		1.0f, -1.0f, -2.0f, -3.0f,
		0.0f, 1.0f, 0.0f, -1.0f,
		-2.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 3.0f, -1.0f
	);

	mat4 result1 = -A;
	mat4 result2 = -B;

	for (std::size_t n = 0; n < mat4::Size; n++) {
		EXPECT_FLOAT_EQ(result1[n], expected1[n]);
	}
	for (std::size_t n = 0; n < mat4::Size; n++) {
		EXPECT_FLOAT_EQ(result2[n], expected2[n]);
	}
}

TEST(MathlibMatrix4, OperationAddition) {
	const float scalar = 2.0f;
	const mat4 A(
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 2.0f, 3.0f, 1.0f
	);
	const mat4 B(
		-1.0f, 1.0f, 2.0f, 3.0f,
		0.0f, -1.0f, 0.0f, 1.0f,
		2.0f, 0.0f, 1.0f, 0.0f,
		0.0f, -1.0f, -3.0f, 1.0f
	);
	mat4 C(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 2.0f,
		0.0f, 0.0f, 1.0f, 3.0f,
		-1.0f, -1.0f, -1.0f, 1.0f
	);

	const mat4 expected1(
		0.0f, 1.0f, 2.0f, 4.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		2.0f, 0.0f, 2.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 2.0f
	);
	const mat4 expected2(
		2.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 2.0f, 0.0f, 2.0f,
		0.0f, 0.0f, 2.0f, 3.0f,
		0.0f, 1.0f, 2.0f, 2.0f
	);
	const mat4 expected3(
		3.0f, 2.0f, 2.0f, 3.0f,
		2.0f, 3.0f, 2.0f, 2.0f,
		2.0f, 2.0f, 3.0f, 2.0f,
		3.0f, 4.0f, 5.0f, 3.0f
	);
	const mat4 expected4(
		4.0f, 2.0f, 2.0f, 3.0f,
		2.0f, 4.0f, 2.0f, 4.0f,
		2.0f, 2.0f, 4.0f, 5.0f,
		2.0f, 3.0f, 4.0f, 4.0f
	);

	mat4 result1 = A + B;
	for (std::size_t n = 0; n < mat4::Size; n++) {
		EXPECT_FLOAT_EQ(result1[n], expected1[n]);
	}
	mat4 result2 = A + scalar;
	for (std::size_t n = 0; n < mat4::Size; n++) {
		EXPECT_FLOAT_EQ(result2[n], expected3[n]);
	}
	C += A;
	for (std::size_t n = 0; n < mat4::Size; n++) {
		EXPECT_FLOAT_EQ(C[n], expected2[n]);
	}
	C += scalar;
	for (std::size_t n = 0; n < mat4::Size; n++) {
		EXPECT_FLOAT_EQ(C[n], expected4[n]);
	}
}

TEST(MathlibMatrix4, OperationSutraction) {
	const float scalar = 2.0f;
	const mat4 A(
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 2.0f, 3.0f, 1.0f
	);
	const mat4 B(
		-1.0f, 1.0f, 2.0f, 3.0f,
		0.0f, -1.0f, 0.0f, 1.0f,
		2.0f, 0.0f, 1.0f, 0.0f,
		0.0f, -1.0f, -3.0f, 1.0f
	);
	mat4 C(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 2.0f,
		0.0f, 0.0f, 1.0f, 3.0f,
		-1.0f, -1.0f, -1.0f, 1.0f
	);

	const mat4 expected1(
		2.0f, -1.0f, -2.0f, -2.0f,
		0.0f, 2.0f, 0.0f, -1.0f,
		-2.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 3.0f, 6.0f, 0.0f
	);
	const mat4 expected2(
		0.0f, 0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 0.0f, 2.0f,
		0.0f, 0.0f, 0.0f, 3.0f,
		-2.0f, -3.0f, -4.0f, 0.0f
	);
	const mat4 expected3(
		-1.0f, -2.0f, -2.0f, -1.0f,
		-2.0f, -1.0f, -2.0f, -2.0f,
		-2.0f, -2.0f, -1.0f, -2.0f,
		-1.0f, 0.0f, 1.0f, -1.0f
	);
	const mat4 expected4(
		-2.0f, -2.0f, -2.0f, -3.0f,
		-2.0f, -2.0f, -2.0f, 0.0f,
		-2.0f, -2.0f, -2.0f, 1.0f,
		-4.0f, -5.0f, -6.0f, -2.0f
	);

	mat4 result1 = A - B;
	for (std::size_t n = 0; n < mat4::Size; n++) {
		EXPECT_FLOAT_EQ(result1[n], expected1[n]);
	}
	mat4 result2 = A - scalar;
	for (std::size_t n = 0; n < mat4::Size; n++) {
		EXPECT_FLOAT_EQ(result2[n], expected3[n]);
	}
	C -= A;
	for (std::size_t n = 0; n < mat4::Size; n++) {
		EXPECT_FLOAT_EQ(C[n], expected2[n]);
	}
	C -= scalar;
	for (std::size_t n = 0; n < mat4::Size; n++) {
		EXPECT_FLOAT_EQ(C[n], expected4[n]);
	}
}

TEST(MathlibMatrix4, OperationMultiplication) {
	const float scalar = 2.0f;
	const vec4 v(3.0f, -2.0f, 1.0f, 0.5f);
	const mat4 A(
		1.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		1.0f, 2.0f, 3.0f, 1.0f
	);
	const mat4 B(
		-1.0f, 0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 2.0f,
		2.0f, 1.0f, -3.0f, 1.0f
	);
	const mat4 expected1(
		-1.0f, 0.0f, -0.5f, -2.0f,
		0.0f, 0.5f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f, -2.0f,
		1.0f, 2.0f, 2.5f, 7.0f
	);
	const mat4 expected2(
		-2.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 2.0f, 0.0f,
		0.0f, 0.0f, 2.0f, 4.0f,
		4.0f, 2.0f, -6.0f, 2.0f
	);
	
	mat4 result1 = A * B;
	mat4 result2 = B * scalar;
	EXPECT_EQ(result1, expected1);
	EXPECT_EQ(result2, expected2);

	mat4 M(
		1.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		1.0f, 2.0f, 3.0f, 1.0f
	);
	mat4 N(
		-1.0f, 0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 2.0f,
		2.0f, 1.0f, -3.0f, 1.0f
	);
	const mat4 expected3(
		-1.0f, 0.0f, -0.5f, -2.0f,
		0.0f, 0.5f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f, -2.0f,
		1.0f, 2.0f, 2.5f, 7.0f
	);
	const mat4 expected4(
		-2.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 2.0f, 0.0f,
		0.0f, 0.0f, 2.0f, 4.0f,
		4.0f, 2.0f, -6.0f, 2.0f
	);

	M *= N;
	N *= scalar;

	EXPECT_EQ(M, expected3);
	EXPECT_EQ(N, expected4);

	const vec4 expected(2.0f, -2.0f, -1.0f, 2.5f);
	vec4 r = A * v;
	EXPECT_EQ(r, expected);
}

TEST(MathlibMatrix4, OperatorComparison) {
	const mat4 A(
		1.0f, 0.001f, 0.0f, 0.1f,
		0.0f, 1.0f, -0.001f, 0.0f,
		0.002f, 0.0f, 1.0f, 0.0f,
		1.0f, 2.0f, 3.0f, 1.0f
	);
	const mat4 B;

	const mat4 expected1(
		1.0f, 0.001f, 0.0f, 0.1f,
		0.0f, 1.0f, -0.001f, 0.0f,
		0.002f, 0.0f, 1.0f, 0.0f,
		1.0f, 2.0f, 3.0f, 1.0f
	);
	const mat4 expected2(
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	);
	
	EXPECT_TRUE(A == expected1);
	EXPECT_FALSE(B != expected2);

	const mat4i a(
		1, 0, 0, 0,
		0, 1, -1, 0,
		0, -0, 0, 0,
		1, 2, 3, 1
	);
	const mat4i b;

	const mat4i expected3(
		1, 0, 0, 0,
		0, 1, -1, 0,
		0, -0, 0, 0,
		1, 2, 3, 1
	);
	const mat4i expected4(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	);

	EXPECT_TRUE(a == expected3);
	EXPECT_FALSE(b != expected4);
}

TEST(MathlibMatrix4, FunctionIdentity) {
	const mat4 result = mat4::Identity();
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

TEST(MathlibMatrix4, FunctionTranspose) {
	const mat4 A(
		1.0f, 0.001f, 0.0f, 0.1f,
		0.0f, 1.0f, -0.001f, 0.0f,
		0.002f, 0.0f, 1.0f, 0.0f,
		1.0f, 2.0f, 3.0f, 1.0f
	);
	const mat4 expected(
		1.0f, 0.0f, 0.002f, 1.0f,
		0.001f, 1.0f, 0.0f, 2.0f,
		0.0f, -0.001f, 1.0f, 3.0f,
		0.1f, 0.0f, 0.0f, 1.0f
	);

	mat4 result = A.Transpose();

	EXPECT_TRUE(result == expected);
}
