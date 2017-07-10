#pragma once

#include <cstdint>

#define UNUSED(x) tinygles::ignore(x)

namespace tinygles
{

template<typename T>
void ignore(T&&) {}

} // namespace tinygles