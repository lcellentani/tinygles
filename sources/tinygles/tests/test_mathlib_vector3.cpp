#include "gtest/gtest.h"

#include "math/Vector3.h"

using namespace mathlib;

TEST(MathlibVector3, DataSize)
{
	const std::size_t result = vec3::Size;
	const std::size_t expected = 3;
	EXPECT_EQ(result, expected);
}