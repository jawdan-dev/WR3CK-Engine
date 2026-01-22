#pragma once
#include <WR3CK/core/core.hpp>

namespace WR3CK
{
class Color {
public:
	Color();
	Color(const uint32_t color);
	Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 0xff);
	Color(const float r, const float g, const float b, const float a = 1.0);
	Color(const Color& other) = default;
	~Color() = default;

	WR3CK_GETTER_MUT(float* const, data, m_data);
	WR3CK_GETTER_MUT(float&, r, m_r);
	WR3CK_GETTER_MUT(float&, g, m_g);
	WR3CK_GETTER_MUT(float&, b, m_b);
	WR3CK_GETTER_MUT(float&, a, m_a);
	WR3CK_GETTER_CONST(uint8_t, r8, static_cast<uint8_t>(m_r * static_cast<float>(0xff)));
	WR3CK_GETTER_CONST(uint8_t, g8, static_cast<uint8_t>(m_g * static_cast<float>(0xff)));
	WR3CK_GETTER_CONST(uint8_t, b8, static_cast<uint8_t>(m_b * static_cast<float>(0xff)));
	WR3CK_GETTER_CONST(uint8_t, a8, static_cast<uint8_t>(m_a * static_cast<float>(0xff)));

	const uint32_t rgba8() const;

private:
	union {
		float m_data[4];
		struct {
			float m_r, m_g, m_b, m_a;
		};
	};
};
}