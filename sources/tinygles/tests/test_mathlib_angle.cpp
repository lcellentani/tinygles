#include "gtest/gtest.h"
#include "math/Angle.h"

using namespace mathlib;

TEST(MathlibAngle, Construction) {
	Angle a;
	EXPECT_EQ(a.degrees(), 0.0f);

	Angle b(3.0f);
	EXPECT_EQ(b.degrees(), 3.0f);
	EXPECT_FLOAT_EQ(b.radians(), 3.0f * Angle::DEG2RAD);

	Angle c(3.0f, Angle::Radian);
	EXPECT_EQ(3.0f, c.radians());
	EXPECT_FLOAT_EQ(c.degrees(), 3.0f * Angle::RAD2DEG);

	const Angle z = Angle::zero();
	EXPECT_EQ(z.degrees(), 0.0f);
	EXPECT_EQ(z.radians(), 0.0f);
}

TEST(MathlibAngle, ComparisonOperator) {
	Angle a(3.0f, Angle::Degree);
	Angle b(3.0f * Angle::DEG2RAD, Angle::Radian);
	EXPECT_EQ(a, b);

	Angle c(3.0f, Angle::Degree);
	Angle d(3.0f, Angle::Radian);
	EXPECT_NE(c, d);

	Angle e(3.0f);
	Angle f(-3.0f);
	EXPECT_TRUE(e > f);
	EXPECT_TRUE(f < e);
}

TEST(MathlibAngle, ArithmeticOperator) {
	Angle a(3.0f, Angle::Degree);
	Angle b(3.0f * Angle::DEG2RAD, Angle::Radian);
	
	const Angle c = a + b;
	float expected1 = 3.0f + 3.0f;
	EXPECT_FLOAT_EQ(c.degrees(), expected1);

	const Angle d = a - b;
	float expected2 = 3.0f - 3.0f;
	EXPECT_FLOAT_EQ(d.degrees(), expected2);

	Angle e(4.0f);
	const Angle result = -e;
	float expected3 = -4.0f;
	EXPECT_FLOAT_EQ(result.degrees(), expected3);
}