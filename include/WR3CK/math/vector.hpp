#pragma once

#include <cstdint>
#include <type_traits>
#include <algorithm>

namespace WR3CK::Math
{
template<size_t t_size>
class Vector {
public:
	Vector();
	template<typename... Args, typename = std::enable_if_t<(sizeof...(Args)) == t_size>>
	Vector(const Args... args);
	Vector(const Vector& other) = default;
	~Vector() = default;

	float* const data() { return (float*)m_data; }
	const float* const data() const { return (float*)m_data; }
	inline const size_t size() const { return t_size; };

#define VECTOR_GETTER(term, index)                                                    \
	template<size_t tt_size = t_size, typename = std::enable_if_t<(index < tt_size)>> \
	inline float& term() { return m_data[index]; };                                   \
	template<size_t tt_size = t_size, typename = std::enable_if_t<(index < tt_size)>> \
	inline const float term() const { return m_data[index]; }

	VECTOR_GETTER(x, 0);
	VECTOR_GETTER(y, 1);
	VECTOR_GETTER(z, 2);
	VECTOR_GETTER(w, 3);
#undef VECTOR_GETTER

	const float dot(const Vector<t_size>& other) const;
	const float lengthSquared() const;
	const float length() const;
	const Vector<t_size> normal() const;
	template<size_t tt_size = t_size, typename = std::enable_if_t<(tt_size == 3)>>
	const Vector<t_size> cross(const Vector<t_size>& other) const;

#define VECTOR_OPERATOR(opSym)                                              \
	const Vector<t_size> operator opSym(const Vector<t_size>& other) const; \
	const Vector<t_size>& operator opSym## = (const Vector<t_size>& other); \
	const Vector<t_size> operator opSym(const float scalar) const;          \
	const Vector<t_size>& operator opSym## = (const float scalar);

	VECTOR_OPERATOR(+);
	VECTOR_OPERATOR(-);
	VECTOR_OPERATOR(*);
	VECTOR_OPERATOR(/);
#undef VECTOR_OPERATOR

	const Vector<t_size> operator-();

	const bool operator==(const Vector<t_size>& other) const;
	const bool operator<(const Vector<t_size>& other) const;

private:
	float m_data[t_size];
};
}

#include "vector.ipp"
