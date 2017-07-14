#include "gtest/gtest.h"

#include "math/Vector4.h"

using namespace mathlib;

TEST(MathlibVector4, DataSize) {
	const std::size_t result = vec4::Size;
	const std::size_t expected = 4;
	EXPECT_EQ(result, expected);
}

TEST(MathlibVector4, DefaultConstructor) {
	const vec4 result;
	const float expected[]{ 0.0f, 0.0f, 0.0f, 0.0f };

	EXPECT_EQ(result[0], expected[0]);
	EXPECT_EQ(result[1], expected[1]);
	EXPECT_EQ(result[2], expected[2]);
	EXPECT_EQ(result[3], expected[3]);
}

TEST(MathlibVector4, ArrayConstructor) {
	const float data[]{ 1.0f, 2.0f, 3.0f, 4.0f };
	const vec4 result(data);
	const float expected[]{ 1.0f, 2.0f, 3.0f, 4.0f };

	EXPECT_EQ(result[0], expected[0]);
	EXPECT_EQ(result[1], expected[1]);
	EXPECT_EQ(result[2], expected[2]);
	EXPECT_EQ(result[3], expected[3]);
}

TEST(MathlibVector4, ScalarConstructor) {
	const vec4 result(1.0f, 2.0f, 3.0f, 4.0f);
	const float expected[]{ 1.0f, 2.0f, 3.0f, 4.0f };

	EXPECT_EQ(result[0], expected[0]);
	EXPECT_EQ(result[1], expected[1]);
	EXPECT_EQ(result[2], expected[2]);
	EXPECT_EQ(result[3], expected[3]);
}

TEST(MathlibVector4, PrebuiltConstructor) {
	const vec4 one = vec4::One();
	const float expected_one[]{ 1.0f, 1.0f, 1.0f, 1.0f };
	EXPECT_EQ(one[0], expected_one[0]);
	EXPECT_EQ(one[1], expected_one[1]);
	EXPECT_EQ(one[2], expected_one[2]);
	EXPECT_EQ(one[3], expected_one[3]);

	const vec4 zero = vec4::Zero();
	const float expected_zero[]{ 0.0f, 0.0f, 0.0f, 0.0f };
	EXPECT_EQ(zero[0], expected_zero[0]);
	EXPECT_EQ(zero[1], expected_zero[1]);
	EXPECT_EQ(zero[2], expected_zero[2]);
	EXPECT_EQ(zero[3], expected_zero[3]);

	const vec4 unitX = vec4::UnitX();
	const float expected_unitX[]{ 1.0f, 0.0f, 0.0f, 0.0f };
	EXPECT_EQ(unitX[0], expected_unitX[0]);
	EXPECT_EQ(unitX[1], expected_unitX[1]);
	EXPECT_EQ(unitX[2], expected_unitX[2]);
	EXPECT_EQ(unitX[3], expected_unitX[3]);

	const vec4 unitY = vec4::UnitY();
	const float expected_unitY[]{ 0.0f, 1.0f, 0.0f, 0.0f };
	EXPECT_EQ(unitY[0], expected_unitY[0]);
	EXPECT_EQ(unitY[1], expected_unitY[1]);
	EXPECT_EQ(unitY[2], expected_unitY[2]);
	EXPECT_EQ(unitY[3], expected_unitY[3]);

	const vec4 unitZ = vec4::UnitZ();
	const float expected_unitZ[]{ 0.0f, 0.0f, 1.0f, 0.0f };
	EXPECT_EQ(unitZ[0], expected_unitZ[0]);
	EXPECT_EQ(unitZ[1], expected_unitZ[1]);
	EXPECT_EQ(unitZ[2], expected_unitZ[2]);
	EXPECT_EQ(unitZ[3], expected_unitZ[3]);

	const vec4 unitW = vec4::UnitW();
	const float expected_unitW[]{ 0.0f, 0.0f, 0.0f, 1.0f };
	EXPECT_EQ(unitW[0], expected_unitW[0]);
	EXPECT_EQ(unitW[1], expected_unitW[1]);
	EXPECT_EQ(unitW[2], expected_unitW[2]);
	EXPECT_EQ(unitW[3], expected_unitW[3]);
}

TEST(MathlibVector4, DataAccessors) {
	const float expected[]{ 1.0f, 2.0f, 3.0f, 4.0f };

	const vec4 const_result(1.0f, 2.0f, 3.0f, 4.0f);
	EXPECT_EQ(const_result.x(), expected[0]);
	EXPECT_EQ(const_result.y(), expected[1]);
	EXPECT_EQ(const_result.z(), expected[2]);
	EXPECT_EQ(const_result.w(), expected[3]);

	vec4 mutable_result(4.0f, 3.0f, 2.0f, 1.0f);
	EXPECT_EQ(mutable_result.x(), expected[3]);
	EXPECT_EQ(mutable_result.y(), expected[2]);
	EXPECT_EQ(mutable_result.z(), expected[1]);
	EXPECT_EQ(mutable_result.w(), expected[0]);
}

TEST(MathlibVector4, OperatorsUnary) {
	const vec4 v1(3.0f, 4.0f, -1.0f, -2.0f);
	const vec4 expected1(-3.0f, -4.0f, 1.0f, 2.0f);
	vec4 result1 = -v1;
	EXPECT_EQ(result1.x(), expected1.x());
	EXPECT_EQ(result1.y(), expected1.y());
	EXPECT_EQ(result1.z(), expected1.z());
	EXPECT_EQ(result1.w(), expected1.w());

	vec4 v2(-1.0f, -2.0f, 4.0f, 3.0f);
	const vec4 expected2(1.0f, 2.0f, -4.0f, -3.0f);
	vec4 result2 = -v2;
	EXPECT_EQ(result2.x(), expected2.x());
	EXPECT_EQ(result2.y(), expected2.y());
	EXPECT_EQ(result2.z(), expected2.z());
	EXPECT_EQ(result2.w(), expected2.w());
}

TEST(MathlibVector4, OperatorsAddition) {
	const float scalar = 2.0f;
	const vec4 v1(1.0, 2.0f, 3.0f, 4.0f);

	const vec4 result1 = v1 + scalar;
	const vec4 expected1(3.0f, 4.0f, 5.0f, 6.0f);
	EXPECT_FLOAT_EQ(result1.x(), expected1.x());
	EXPECT_FLOAT_EQ(result1.y(), expected1.y());
	EXPECT_FLOAT_EQ(result1.z(), expected1.z());
	EXPECT_FLOAT_EQ(result1.w(), expected1.w());

	vec4 v2(3.0f, 2.0f, 1.0f, 0.0f);
	v2 += scalar;
	const vec4 expected2(5.0f, 4.0f, 3.0f, 2.0f);
	EXPECT_FLOAT_EQ(v2.x(), expected2.x());
	EXPECT_FLOAT_EQ(v2.y(), expected2.y());
	EXPECT_FLOAT_EQ(v2.z(), expected2.z());
	EXPECT_FLOAT_EQ(v2.w(), expected2.w());

	const vec4 result3 = v1 + v2;
	const vec4 expected3(6.0f, 6.0f, 6.0f, 6.0f);
	EXPECT_FLOAT_EQ(result3.x(), expected3.x());
	EXPECT_FLOAT_EQ(result3.y(), expected3.y());
	EXPECT_FLOAT_EQ(result3.z(), expected3.z());
	EXPECT_FLOAT_EQ(result3.w(), expected3.w());

	vec4 v4(3.0f, 3.0f, 1.0f, -1.0f);
	v4 += v2;
	const vec4 expected4(8.0f, 7.0f, 4.0f, 1.0f);
	EXPECT_FLOAT_EQ(v4.x(), expected4.x());
	EXPECT_FLOAT_EQ(v4.y(), expected4.y());
	EXPECT_FLOAT_EQ(v4.z(), expected4.z());
	EXPECT_FLOAT_EQ(v4.w(), expected4.w());
}

TEST(MathlibVector4, OperatorsSubtraction) {
	const float scalar = 2.0f;
	const vec4 v1(1.0, 2.0f, 3.0f, 4.0f);

	const vec4 result1 = v1 - scalar;
	const vec4 expected1(-1.0f, 0.0f, 1.0f, 2.0f);
	EXPECT_FLOAT_EQ(result1.x(), expected1.x());
	EXPECT_FLOAT_EQ(result1.y(), expected1.y());
	EXPECT_FLOAT_EQ(result1.z(), expected1.z());
	EXPECT_FLOAT_EQ(result1.w(), expected1.w());

	vec4 v2(3.0f, 2.0f, 1.0f, 0.0f);
	v2 -= scalar;
	const vec4 expected2(1.0f, 0.0f, -1.0f, -2.0f);
	EXPECT_FLOAT_EQ(v2.x(), expected2.x());
	EXPECT_FLOAT_EQ(v2.y(), expected2.y());
	EXPECT_FLOAT_EQ(v2.z(), expected2.z());
	EXPECT_FLOAT_EQ(v2.w(), expected2.w());

	const vec4 result3 = v1 - v2;
	const vec4 expected3(0.0f, 2.0f, 4.0f, 6.0f);
	EXPECT_FLOAT_EQ(result3.x(), expected3.x());
	EXPECT_FLOAT_EQ(result3.y(), expected3.y());
	EXPECT_FLOAT_EQ(result3.z(), expected3.z());
	EXPECT_FLOAT_EQ(result3.w(), expected3.w());

	vec4 v4(3.0f, 3.0f, 1.0f, -1.0f);
	v4 -= v2;
	const vec4 expected4(2.0f, 3.0f, 2.0f, 1.0f);
	EXPECT_FLOAT_EQ(v4.x(), expected4.x());
	EXPECT_FLOAT_EQ(v4.y(), expected4.y());
	EXPECT_FLOAT_EQ(v4.z(), expected4.z());
	EXPECT_FLOAT_EQ(v4.w(), expected4.w());
}

TEST(MathlibVector4, OperatorsMultiplication) {
	const float scalar = 2.0f;
	const vec4 v1(1.0, 2.0f, 3.0f, 4.0f);

	const vec4 result1 = v1 * scalar;
	const vec4 expected1(2.0f, 4.0f, 6.0f, 8.0f);
	EXPECT_FLOAT_EQ(result1.x(), expected1.x());
	EXPECT_FLOAT_EQ(result1.y(), expected1.y());
	EXPECT_FLOAT_EQ(result1.z(), expected1.z());
	EXPECT_FLOAT_EQ(result1.w(), expected1.w());

	vec4 v2(3.0f, 2.0f, -1.0f, -2.0f);
	v2 *= scalar;
	const vec4 expected2(6.0f, 4.0f, -2.0f, -4.0f);
	EXPECT_FLOAT_EQ(v2.x(), expected2.x());
	EXPECT_FLOAT_EQ(v2.y(), expected2.y());
	EXPECT_FLOAT_EQ(v2.z(), expected2.z());
	EXPECT_FLOAT_EQ(v2.w(), expected2.w());

	const vec4 result3 = v1 * v2;
	const vec4 expected3(6.0f, 8.0f, -6.0f, -16.0f);
	EXPECT_FLOAT_EQ(result3.x(), expected3.x());
	EXPECT_FLOAT_EQ(result3.y(), expected3.y());
	EXPECT_FLOAT_EQ(result3.z(), expected3.z());
	EXPECT_FLOAT_EQ(result3.w(), expected3.w());

	vec4 v4(3.0f, 3.0f, 1.0f, -2.0f);
	v4 *= v2;
	const vec4 expected4(18.0f, 12.0f, -2.0f, 8.0f);
	EXPECT_FLOAT_EQ(v4.x(), expected4.x());
	EXPECT_FLOAT_EQ(v4.y(), expected4.y());
	EXPECT_FLOAT_EQ(v4.z(), expected4.z());
	EXPECT_FLOAT_EQ(v4.w(), expected4.w());
}

TEST(MathlibVector4, OperatorsDivision) {
	const float scalar = 2.0f;
	const vec4 v1(1.0, 2.0f, 3.0f, 4.0f);

	const vec4 result1 = v1 / scalar;
	const vec4 expected1(0.5f, 1.0f, 1.5f, 2.0f);
	EXPECT_FLOAT_EQ(result1.x(), expected1.x());
	EXPECT_FLOAT_EQ(result1.y(), expected1.y());
	EXPECT_FLOAT_EQ(result1.z(), expected1.z());
	EXPECT_FLOAT_EQ(result1.w(), expected1.w());

	vec4 v2(3.0f, 2.0f, -1.0f, -2.0f);
	v2 /= scalar;
	const vec4 expected2(1.5f, 1.0f, -0.5f, -1.0f);
	EXPECT_FLOAT_EQ(v2.x(), expected2.x());
	EXPECT_FLOAT_EQ(v2.y(), expected2.y());
	EXPECT_FLOAT_EQ(v2.z(), expected2.z());
	EXPECT_FLOAT_EQ(v2.w(), expected2.w());

	const vec4 result3 = v1 / v2;
	const vec4 expected3((1.0f / 1.5f), 2.0f, (3.0f / -0.5f), -4.0f);
	EXPECT_FLOAT_EQ(result3.x(), expected3.x());
	EXPECT_FLOAT_EQ(result3.y(), expected3.y());
	EXPECT_FLOAT_EQ(result3.z(), expected3.z());
	EXPECT_FLOAT_EQ(result3.w(), expected3.w());

	vec4 v4(3.0f, 3.0f, 1.0f, -2.0f);
	v4 /= v2;
	const vec4 expected4((3.0f / 1.5f), 3.0f, -2.0f, 2.0f);
	EXPECT_FLOAT_EQ(v4.x(), expected4.x());
	EXPECT_FLOAT_EQ(v4.y(), expected4.y());
	EXPECT_FLOAT_EQ(v4.z(), expected4.z());
	EXPECT_FLOAT_EQ(v4.w(), expected4.w());
}

TEST(MathlibVector4, OperatorsComparison) {
	vec4 a1(0.0f, 0.0f, 0.0f, 0.01f);
	vec4 b1(0.0f, 0.0f, 0.0f, 0.01f);
	EXPECT_EQ(a1, b1);

	vec4 a2(1.0f, 2.0f, 3.0f, 0.0f);
	vec4 b2(1.0f, 2.0f, -3.0f, 0.01f);
	EXPECT_NE(a2, b2);

	const vec4i i1(1, 2, 3, -1);
	const vec4i j1(1, 2, 3, -1);
	EXPECT_EQ(i1, j1);

	vec4 a3(0.0f, 0.0f, 1.0f, -1.0f);
	vec4 b3(-1.0f, -1.0f, 0.0f, -2.0f);
	EXPECT_TRUE(a3 > b3);
}

TEST(MathlibVector4, FunctionDot) {
	const vec4 a(3.0f, 6.0f, 1.0f, -1.0f);
	const vec4 b(5.0f, 2.0f, 7.0f, 3.0f);

	const float result = vec4::dot(a, b);
	const float expected = 31.0f;
	EXPECT_FLOAT_EQ(result, expected);
}

TEST(MathlibVector4, FunctionMinMax) {
	const vec4 a1(-1.0f, 1.0f, 2.0f, 5.0f);
	const vec4 b1(0.0f, -1.0f, -3.0f, 4.0f);
	const vec4 expected1(-1.0f, -1.0f, -3.0f, 4.0f);
	const vec4 result1 = vec4::min(a1, b1);
	EXPECT_EQ(result1, expected1);

	const vec4 a2(-1.0f, 1.0f, 2.0f, 5.0f);
	const vec4 b2(0.0f, -1.0f, -3.0f, 4.0f);
	const vec4 expected2(0.0f, 1.0f, 2.0f, 5.0f);
	const vec4 result2 = vec4::max(a2, b2);
	EXPECT_EQ(result2, expected2);
}

TEST(MathlibVector4, FunctionMagnitude) {
	const vec4 data(1.0f, 2.0f, 3.0f, -1.0f);
	const float result1 = data.magnitude();
	const float expected1 = std::sqrt(15.0f);
	EXPECT_FLOAT_EQ(result1, expected1);

	const vec4 unitX = vec4::UnitX();
	const float result2 = unitX.magnitude();
	const float expected2 = 1.0f;
	EXPECT_FLOAT_EQ(result2, expected2);
}

TEST(MathlibVector4, FunctionNormalize) {
	const float scalar = 2.0f / std::sqrt(16.0f);
	vec4 data(2.0f, 2.0f, 2.0f, 2.0f);

	const vec4 result1 = data.normalize();
	vec4 result2 = data.normalized();

	const vec4 expected(scalar, scalar, scalar, scalar);
	EXPECT_EQ(result1, expected);
	EXPECT_EQ(result2, expected);
}