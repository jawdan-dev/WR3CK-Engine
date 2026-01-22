#include <WR3CK/math/color.hpp>

namespace WR3CK
{
Color::Color() : Color(0.0f, 0.0f, 0.0f, 1.0f) {}
Color::Color(const uint32_t color) :
	Color(
		static_cast<uint8_t>((color >> 0) & 0xff),
		static_cast<uint8_t>((color >> 8) & 0xff),
		static_cast<uint8_t>((color >> 16) & 0xff),
		static_cast<uint8_t>((color >> 24) & 0xff)
	) {}
Color::Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) :
	Color(
		static_cast<float>(r) / static_cast<float>(0xff),
		static_cast<float>(g) / static_cast<float>(0xff),
		static_cast<float>(b) / static_cast<float>(0xff),
		static_cast<float>(a) / static_cast<float>(0xff)
	) {}
Color::Color(const float r, const float g, const float b, const float a) :
	m_r(r), m_g(g), m_b(b), m_a(a) {}

const uint32_t Color::rgba8() const {
	return (static_cast<uint32_t>(r8()) << 0) |
		   (static_cast<uint32_t>(g8()) << 8) |
		   (static_cast<uint32_t>(b8()) << 16) |
		   (static_cast<uint32_t>(a8()) << 24);
}
}