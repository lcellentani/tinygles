#include "gtest/gtest.h"

#include "math/Vector3.h"

using namespace mathlib;

TEST(MathlibVector3, DataSize)
{
	const std::size_t result = vec3::Size;
	const std::size_t expected = 3;
	EXPECT_EQ(result, expected);
}

TEST(MathlibVector3, DefaultConstructor) {
	const vec3 result;
	const float expected[]{ 0.0f, 0.0f, 0.0f };

	EXPECT_EQ(result[0], expected[0]);
	EXPECT_EQ(result[1], expected[1]);
	EXPECT_EQ(result[2], expected[2]);
}

TEST(MathlibVector3, ArrayConstructor) {
	const float data[]{ 1.0f, 2.0f, 3.0f };
	const vec3 result(data);
	const float expected[]{ 1.0f, 2.0f, 3.0f };

	EXPECT_EQ(result[0], expected[0]);
	EXPECT_EQ(result[1], expected[1]);
	EXPECT_EQ(result[2], expected[2]);
}

TEST(MathlibVector3, ScalarConstructor) {
	const vec3 result(1.0f, 2.0f, 3.0f);
	const float expected[]{ 1.0f, 2.0f, 3.0f };

	EXPECT_EQ(result[0], expected[0]);
	EXPECT_EQ(result[1], expected[1]);
	EXPECT_EQ(result[2], expected[2]);
}

TEST(MathlibVector3, PrebuiltConstructor) {
	const vec3 one = vec3::One();
	const float expected_one[]{ 1.0f, 1.0f, 1.0f };
	EXPECT_EQ(one[0], expected_one[0]);
	EXPECT_EQ(one[1], expected_one[1]);
	EXPECT_EQ(one[2], expected_one[2]);

	const vec3 zero = vec3::Zero();
	const float expected_zero[]{ 0.0f, 0.0f, 0.0f };
	EXPECT_EQ(zero[0], expected_zero[0]);
	EXPECT_EQ(zero[1], expected_zero[1]);
	EXPECT_EQ(zero[2], expected_zero[2]);

	const vec3 unitX = vec3::UnitX();
	const float expected_unitX[]{ 1.0f, 0.0f, 0.0f };
	EXPECT_EQ(unitX[0], expected_unitX[0]);
	EXPECT_EQ(unitX[1], expected_unitX[1]);
	EXPECT_EQ(unitX[2], expected_unitX[2]);

	const vec3 unitY = vec3::UnitY();
	const float expected_unitY[]{ 0.0f, 1.0f, 0.0f };
	EXPECT_EQ(unitY[0], expected_unitY[0]);
	EXPECT_EQ(unitY[1], expected_unitY[1]);
	EXPECT_EQ(unitY[2], expected_unitY[2]);

	const vec3 unitZ = vec3::UnitZ();
	const float expected_unitZ[]{ 0.0f, 0.0f, 1.0f };
	EXPECT_EQ(unitZ[0], expected_unitZ[0]);
	EXPECT_EQ(unitZ[1], expected_unitZ[1]);
	EXPECT_EQ(unitZ[2], expected_unitZ[2]);
}

TEST(MathlibVector3, DataAccessors) {
	const float expected[]{ 1.0f, 2.0f, 3.0f };
	
	const vec3 const_result(1.0f, 2.0f, 3.0f);
	EXPECT_EQ(const_result.x(), expected[0]);
	EXPECT_EQ(const_result.y(), expected[1]);
	EXPECT_EQ(const_result.z(), expected[2]);

	vec3 mutable_result(3.0f, 2.0f, 1.0f);
	EXPECT_EQ(mutable_result.x(), expected[2]);
	EXPECT_EQ(mutable_result.y(), expected[1]);
	EXPECT_EQ(mutable_result.z(), expected[0]);
}

TEST(MathlibVector3, OperatorsAddition) {
	const float scalar = 2.0f;
	const vec3 v1(1.0, 2.0f, 3.0f);

	const vec3 result1 = v1 + scalar;
	const vec3 expected1(3.0f, 4.0f, 5.0f);
	EXPECT_FLOAT_EQ(result1.x(), expected1.x());
	EXPECT_FLOAT_EQ(result1.y(), expected1.y());
	EXPECT_FLOAT_EQ(result1.z(), expected1.z());

	vec3 v2(3.0f, 2.0f, 1.0f);
	v2 += scalar;
	const vec3 expected2(5.0f, 4.0f, 3.0f);
	EXPECT_FLOAT_EQ(v2.x(), expected2.x());
	EXPECT_FLOAT_EQ(v2.y(), expected2.y());
	EXPECT_FLOAT_EQ(v2.z(), expected2.z());

	const vec3 result3 = v1 + v2;
	const vec3 expected3(6.0f, 6.0f, 6.0f);
	EXPECT_FLOAT_EQ(result3.x(), expected3.x());
	EXPECT_FLOAT_EQ(result3.y(), expected3.y());
	EXPECT_FLOAT_EQ(result3.z(), expected3.z());

	vec3 v4(3.0f, 3.0f, 1.0f);
	v4 += v2;
	const vec3 expected4(8.0f, 7.0f, 4.0f);
	EXPECT_FLOAT_EQ(v4.x(), expected4.x());
	EXPECT_FLOAT_EQ(v4.y(), expected4.y());
	EXPECT_FLOAT_EQ(v4.z(), expected4.z());
}

TEST(MathlibVector3, OperatorsSubtraction) {
	const float scalar = 2.0f;
	const vec3 v1(1.0, 2.0f, 3.0f);

	const vec3 result1 = v1 - scalar;
	const vec3 expected1(-1.0f, 0.0f, 1.0f);
	EXPECT_FLOAT_EQ(result1.x(), expected1.x());
	EXPECT_FLOAT_EQ(result1.y(), expected1.y());
	EXPECT_FLOAT_EQ(result1.z(), expected1.z());

	vec3 v2(3.0f, 2.0f, 1.0f);
	v2 -= scalar;
	const vec3 expected2(1.0f, 0.0f, -1.0f);
	EXPECT_FLOAT_EQ(v2.x(), expected2.x());
	EXPECT_FLOAT_EQ(v2.y(), expected2.y());
	EXPECT_FLOAT_EQ(v2.z(), expected2.z());

	const vec3 result3 = v1 - v2;
	const vec3 expected3(0.0f, 2.0f, 4.0f);
	EXPECT_FLOAT_EQ(result3.x(), expected3.x());
	EXPECT_FLOAT_EQ(result3.y(), expected3.y());
	EXPECT_FLOAT_EQ(result3.z(), expected3.z());

	vec3 v4(3.0f, 3.0f, 1.0f);
	v4 -= v2;
	const vec3 expected4(2.0f, 3.0f, 2.0f);
	EXPECT_FLOAT_EQ(v4.x(), expected4.x());
	EXPECT_FLOAT_EQ(v4.y(), expected4.y());
	EXPECT_FLOAT_EQ(v4.z(), expected4.z());
}

TEST(MathlibVector3, OperatorsMultiplication) {
	const float scalar = 2.0f;
	const vec3 v1(1.0, 2.0f, 3.0f);

	const vec3 result1 = v1 * scalar;
	const vec3 expected1(2.0f, 4.0f, 6.0f);
	EXPECT_FLOAT_EQ(result1.x(), expected1.x());
	EXPECT_FLOAT_EQ(result1.y(), expected1.y());
	EXPECT_FLOAT_EQ(result1.z(), expected1.z());

	vec3 v2(3.0f, 2.0f, -1.0f);
	v2 *= scalar;
	const vec3 expected2(6.0f, 4.0f, -2.0f);
	EXPECT_FLOAT_EQ(v2.x(), expected2.x());
	EXPECT_FLOAT_EQ(v2.y(), expected2.y());
	EXPECT_FLOAT_EQ(v2.z(), expected2.z());

	const vec3 result3 = v1 * v2;
	const vec3 expected3(6.0f, 8.0f, -6.0f);
	EXPECT_FLOAT_EQ(result3.x(), expected3.x());
	EXPECT_FLOAT_EQ(result3.y(), expected3.y());
	EXPECT_FLOAT_EQ(result3.z(), expected3.z());

	vec3 v4(3.0f, 3.0f, 1.0f);
	v4 *= v2;
	const vec3 expected4(18.0f, 12.0f, -2.0f);
	EXPECT_FLOAT_EQ(v4.x(), expected4.x());
	EXPECT_FLOAT_EQ(v4.y(), expected4.y());
	EXPECT_FLOAT_EQ(v4.z(), expected4.z());
}

TEST(MathlibVector3, OperatorsDivision) {
	const float scalar = 2.0f;
	const vec3 v1(1.0, 2.0f, 3.0f);

	const vec3 result1 = v1 / scalar;
	const vec3 expected1(0.5f, 1.0f, 1.5f);
	EXPECT_FLOAT_EQ(result1.x(), expected1.x());
	EXPECT_FLOAT_EQ(result1.y(), expected1.y());
	EXPECT_FLOAT_EQ(result1.z(), expected1.z());

	vec3 v2(3.0f, 2.0f, -1.0f);
	v2 /= scalar;
	const vec3 expected2(1.5f, 1.0f, -0.5);
	EXPECT_FLOAT_EQ(v2.x(), expected2.x());
	EXPECT_FLOAT_EQ(v2.y(), expected2.y());
	EXPECT_FLOAT_EQ(v2.z(), expected2.z());

	const vec3 result3 = v1 / v2;
	const vec3 expected3((1.0f / 1.5f), 2.0f, -6.0f);
	EXPECT_FLOAT_EQ(result3.x(), expected3.x());
	EXPECT_FLOAT_EQ(result3.y(), expected3.y());
	EXPECT_FLOAT_EQ(result3.z(), expected3.z());

	vec3 v4(3.0f, 3.0f, 1.0f);
	v4 /= v2;
	const vec3 expected4((3.0f / 1.5f), 3.0f, -2.0f);
	EXPECT_FLOAT_EQ(v4.x(), expected4.x());
	EXPECT_FLOAT_EQ(v4.y(), expected4.y());
	EXPECT_FLOAT_EQ(v4.z(), expected4.z());
}

TEST(MathlibVector3, OperatorsUnary) {
	const vec3 v1(3.0f, 4.0f, -1.0f);
	const vec3 expected1(-3.0f, -4.0f, 1.0f);
	vec3 result1 = -v1;
	EXPECT_EQ(result1.x(), expected1.x());
	EXPECT_EQ(result1.y(), expected1.y());
	EXPECT_EQ(result1.z(), expected1.z());

	vec3 v2(-1.0f, -2.0f, 4.0f);
	const vec3 expected2(1.0f, 2.0f, -4.0f);
	vec3 result2 = -v2;
	EXPECT_EQ(result2.x(), expected2.x());
	EXPECT_EQ(result2.y(), expected2.y());
	EXPECT_EQ(result2.z(), expected2.z());
}