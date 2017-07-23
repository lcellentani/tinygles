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

TEST(MathlibMatrix4, OperationDivision) {
	const float scalar = 2.0f;
	const mat4 A(
		1.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		1.0f, 2.0f, 3.0f, 1.0f
	);
	mat4 B(
		-1.0f, 0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 2.0f,
		2.0f, 1.0f, -3.0f, 1.0f
	);
	const mat4 expected1(
		0.5f, 0.0f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, -0.5f, 0.0f,
		0.5f, 1.0f, 1.5f, 0.5f
	);
	const mat4 expected2(
		-0.5f, 0.0f, 0.25f, 0.0f,
		0.0f, 0.25f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.5f, 1.0f,
		1.0f, 0.5f, -1.5f, 0.5f
	);

	mat4 result1 = A / scalar;
	B /= scalar;
	EXPECT_EQ(result1, expected1);
	EXPECT_EQ(B, expected2);
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

TEST(MathlibMatrix4, FunctionFromTranslationVector) {
	const float data[3]{ 1.0f, -2.0f, 3.0f };
	mat4 A = mat4::FromTranslationVector(data);
	mat4 B = mat4::FromTranslationVector(-2.0f, 3.5f, -1.5f);

	const mat4 expected1(
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, -2.0f,
		0.0f, 0.0f, 1.0f, 3.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	const mat4 expected2(
		1.0f, 0.0f, 0.0f, -2.0f,
		0.0f, 1.0f, 0.0f, 3.5f,
		0.0f, 0.0f, 1.0f, -1.5f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	EXPECT_EQ(A, expected1);
	EXPECT_EQ(B, expected2);
}

TEST(MathlibMatrix4, FunctionFromScaleVector) {
	const float data[3]{ 1.0f, -2.0f, 3.0f };
	mat4 A = mat4::FromScaleVector(data);
	mat4 B = mat4::FromScaleVector(-2.0f, 3.5f, -1.5f);

	const mat4 expected1(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 3.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	const mat4 expected2(
		-2.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 3.5f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.5f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	EXPECT_EQ(A, expected1);
	EXPECT_EQ(B, expected2);
}

TEST(MathlibMatrix4, FunctionRotationMatrix) {
	const float angle = 30.0f;
	const vec3 axisX(1.0f, 0.0f, 0.0f);
	const vec3 axisY(0.0f, 1.0f, 0.0f);
	const vec3 axisZ(0.0f, 0.0f, 1.0f);
	
	mat4 result1 = mat4::FromRotation(angle, axisX);
	mat4 exptected1 = mat4::RotationX(angle);
	EXPECT_EQ(result1, exptected1);

	mat4 result2 = mat4::FromRotation(angle, axisY);
	mat4 exptected2 = mat4::RotationY(angle);
	EXPECT_EQ(result2, exptected2);

	mat4 result3 = mat4::FromRotation(angle, axisZ);
	mat4 exptected3 = mat4::RotationZ(angle);
	EXPECT_EQ(result3, exptected3);
}

TEST(MathlibMatrix4, FunctionPerspectiveProjection) {
	const mat4 perspNormalizedRH = PerspectiveMatrix(std::atan(1.0f) * 2.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	const mat4 expected1(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, -1,
		0, 0, 0, 0);
	EXPECT_EQ(perspNormalizedRH, expected1);

	const mat4 perspNormalizedLH = PerspectiveMatrix(std::atan(1.0f) * 2.0f, 1.0f, 0.0f, 1.0f, -1.0f);
	const mat4 expected2(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 1,
		0, 0, 0, 0);
	EXPECT_EQ(perspNormalizedLH, expected2);

	const mat4 perspWideFOV = PerspectiveMatrix(std::atan(2.0f) * 2.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	const mat4 expected3(
		0.5, 0, 0, 0,
		0, 0.5, 0, 0,
		0, 0, -1, -1,
		0, 0, 0, 0);
	EXPECT_EQ(perspWideFOV, expected3);

	const mat4 perspNarrowFOV = PerspectiveMatrix(std::atan(0.1f) * 2.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	const mat4 expected4(
		10, 0, 0, 0,
		0, 10, 0, 0,
		0, 0, -1, -1,
		0, 0, 0, 0);
	EXPECT_EQ(perspNarrowFOV, expected4);

	const mat4 perspAspectRatio = PerspectiveMatrix(std::atan(1.0f) * 2.0f, 0.5f, 0.0f, 1.0f, 1.0f);
	const mat4 expected5(
		2, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, -1,
		0, 0, 0, 0);
	EXPECT_EQ(perspAspectRatio, expected5);

	const mat4 perspDeeperViewFrustum = PerspectiveMatrix(std::atan(1.0f) * 2.0f, 1.0f, -2.0f, 2.0f, 1.0f);
	const mat4 expected6(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -0.5, -1,
		0, 0, 2, 0);
	EXPECT_EQ(perspDeeperViewFrustum, expected6);
}

TEST(MathlibMatrix4, FunctionOrthoProjection) {
	const mat4 orthoNormalized = OrthoMatrix(0.0f, 2.0f, 0.0f, 2.0f, 2.0f, 0.0f);
	const mat4 expected1(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-1, -1, 1, 1);
	EXPECT_EQ(orthoNormalized, expected1);

	const mat4 orthoNormalizedRH = OrthoMatrix(0.0f, 2.0f, 0.0f, 2.0f, 2.0f, 0.0f, 1.0f);
	const mat4 expected2(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-1, -1, 1, 1);
	EXPECT_EQ(orthoNormalizedRH, expected2);

	const mat4 orthoNarrowRH = OrthoMatrix(1.0f, 3.0f, 0.0f, 2.0f, 2.0f, 0.0f, 1.0f);
	const mat4 expected3(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-2, -1, 1, 1);
	EXPECT_EQ(orthoNarrowRH, expected3);

	const mat4 orthoSquatRH = OrthoMatrix(0.0f, 2.0f, 1.0f, 3.0f, 2.0f, 0.0f, 1.0f);
	const mat4 expected4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-1, -2, 1, 1);
	EXPECT_EQ(orthoSquatRH, expected4);

	const mat4 orthoDeepRH = OrthoMatrix(0.0f, 2.0f, 0.0f, 2.0f, 3.0f, 1.0f, 1.0f);
	const mat4 expected5(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-1, -1, 2, 1);
	EXPECT_EQ(orthoDeepRH, expected5);

	const mat4 orthoNormalizedLH = OrthoMatrix(0.0f, 2.0f, 0.0f, 2.0f, 2.0f, 0.0f, -1.0f);
	const mat4 expected6(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		-1, -1, 1, 1);
	EXPECT_EQ(orthoNormalizedLH, expected6);

	const mat4 orthoCanonicalLH = OrthoMatrix(1.0f, 3.0f, 1.0f, 3.0f, 1.0f, 3.0f, -1.0f);
	const mat4 expected7(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-2, -2, -2, 1);
	EXPECT_EQ(orthoCanonicalLH, expected7);
}

TEST(MathlibMatrix4, FunctionLookAt) {
	const mat4 originAlongZ_1 = LookAt(vec3(0, 0, 1), vec3(0, 0, 0), vec3(0, 1, 0));
	const mat4 expected1(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	EXPECT_EQ(originAlongZ_1, expected1);

	const mat4 originAlongDiagonal = LookAt(vec3(0, 0, 0), vec3(1, 1, 1), vec3(0, 1, 0));
	const mat4 expected2(
		-0.707106781f,            0,  0.707106781f,             0,
		-0.408248290f, 0.816496580f, -0.408248290f,             0,
		-0.577350269f, -0.577350269f, -0.577350269f, 1.732050808f,
		            0,             0,             0,            1);
	EXPECT_EQ(originAlongDiagonal, expected2);

	const mat4 originAlongZ_2 = LookAt(vec3(0, 0, 2), vec3(0, 0, 0), vec3(0, 1, 0));
	const mat4 expected3(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	EXPECT_EQ(originAlongZ_2, expected3);

	const mat4 originAlongX = LookAt(vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0));
	const mat4 expected4(
		0, 0, -1, 0,
		0, 1, 0, 0,
		1, 0, 0, 0,
		0, 0, 0, 1);
	EXPECT_EQ(originAlongX, expected4);
}
