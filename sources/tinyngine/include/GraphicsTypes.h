#pragma once

#include <cstdint>

#define SHADER_SOURCE(...) #__VA_ARGS__

namespace tinyngine
{
	struct TextureFormats {
		enum Enum {
			RGB8,
			RGBA8,
			BGRA8,
			Count
		};
	};
	struct Attributes {
		enum Enum {
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
	};

	struct AttributeType {
		enum Enum {
			Uint8,
			Int16,
			Float,
			Count
		};
	};

	struct UniformType {
		enum Enum {
			Float,
			Float2,
			Float3,
			Float4,
			Int,
			Int2,
			Int3,
			Int4,
			Bool,
			Bool2,
			Bool3,
			Bool4,
			Mat2,
			Mat3,
			Mat4,
			Sampler2D,
			SamplerCube,
			Count
		};
	};

	struct Uniform {
		uint32_t mLocation;
		uint16_t mSize;
		UniformType::Enum mType;
		char mName[256];
	};

	struct ShaderType {
		enum Enum {
			VertexProgram,
			FragmentProgram,
			Count
		};
	};

	struct PrimitiveType {
		enum Enum {
			Triangles,
			TriangleStrip,
			Lines,
			LineStrip,
			Point,
			Count
		};
	};

	struct RendererStateType {
		enum Enum {
			Blend,
			CullFace,
			DepthTest,
			PolygonOffset,
			Scissor,
			Stencil,
			Count
		};
	};

	struct BlendFuncs {
		enum Enum {
			Zero,
			One,
			SrcColor,
			OneMinusSrcColor,
			DstColor,
			OneMinusDstColor,
			SrcAlpha,
			OneMinusSrcAlpha,
			DstAlpha,
			OneMinusDstAlpha,
			ConstantColor,
			OneMinusConstantColor,
			ConstantAlpha,
			OneMinusConstantAlpha,
			SrcAlphaSaturate,
			Count
		};
	};

	struct CullFaceModes {
		enum Enum {
			Front,
			Back,
			FrontAndBack,
			Count
		};
	};

	struct WindingModes {
		enum Enum {
			ClockWise,
			CounterClockWise,
			Count
		};
	};

	struct DepthFuncs {
		enum Enum {
			Never,
			Less,
			Equal,
			LessEqual,
			Greater,
			NotEqual,
			GreaterEqual,
			Always,
			Count
		};
	};

	struct StencilFuncs {
		enum Enum {
			Never,
			Less,
			Equal,
			LessEqual,
			Greater,
			NotEqual,
			GreaterEqual,
			Always,
			Count
		};
	};

	struct StencilOpTypes {
		enum Enum {
			Keep,
			Zero,
			Replace,
			Increment,
			IncrementWrap,
			Decrement,
			DecrementWrap,
			Invert,
			Count
		};
	};

	struct ImageData {
		uint32_t mWidth;
		uint32_t mHeight;
		uint32_t mSize;
		uint8_t mBitsPerPixel;
		void* mData;
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
	using UniformHandle = ResourceHandle;
	using TextureHandle = ResourceHandle;
	using ImageHandle = ResourceHandle;

} // namespace tinyngine