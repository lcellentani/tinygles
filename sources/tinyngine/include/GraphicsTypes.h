#pragma once

#include <cstdint>

#define SHADER_SOURCE(...) #__VA_ARGS__

namespace tinyngine
{
	enum class Attributes : uint8_t {
		Position,
		Normal,
		Tangent,
		Bitangent,
		Color0,
		Color1,
		TexCoord0,
		TexCoord1,
		TexCoord2,
		TexCoord3,
		Count
	};

	enum class AttributeType : uint8_t {
		Uint8,
		Int16,
		Float,
		Count
	};

	enum class ShaderType {
		VertexProgram,
		FragmentProgram,
		Count
	};

	struct Color
	{
		static constexpr Color Red() { return Color(255, 0, 0); }
		static constexpr Color Green() { return Color(0, 255, 0); }
		static constexpr Color Blue() { return Color(0, 0, 255); }
		static constexpr Color Black() { return Color(0, 0, 0); }
		static constexpr Color White() { return Color(255, 255, 255); }

		constexpr Color() : r(0), g(0), b(0), a(255) {}
		constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}

		constexpr const float red() const { return static_cast<float>(r / 256.0f); }
		constexpr const float green() const { return static_cast<float>(g / 256.0f); }
		constexpr const float blue() const { return static_cast<float>(b / 256.0f); }
		constexpr const float alpha() const { return static_cast<float>(a / 256.0f); }

		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};

	static const uint32_t cInvalidHandle = UINT32_MAX;

	struct ResourceHandle {
		ResourceHandle() = default;
		ResourceHandle(uint32_t handle) : mHandle(handle) {}

		ResourceHandle& operator=(const ResourceHandle&) = default;

		inline const bool IsValid() const { return mHandle != cInvalidHandle; }

		uint32_t mHandle = 0;
	};
	
	using ShaderHandle = ResourceHandle;
	using ProgramHandle = ResourceHandle;
	using VertexBufferHandle = ResourceHandle;
	using IndexBufferHandle = ResourceHandle;

} // namespace tinyngine