#pragma once
#include "vector.hpp"

#include <cmath>

namespace WR3CK::Math
{
template<size_t t_size>
Vector<t_size>::Vector() {
	for (size_t i = 0; i < t_size; i++) {
		m_data[i] = 0;
	}
}
template<size_t t_size>
template<typename... Args, typename>
Vector<t_size>::Vector(const Args... args) {
	const float vec[t_size] { (float)args... };
	for (size_t i = 0; i < t_size; i++) {
		m_data[i] = vec[i];
	}
}

template<size_t t_size>
const float Vector<t_size>::dot(const Vector<t_size>& other) const {
	float dotProduct = 0;
	for (size_t i = 0; i < t_size; i++) {
		dotProduct += m_data[i] * other.m_data[i];
	}
	return dotProduct;
}
template<size_t t_size>
const float Vector<t_size>::lengthSquared() const {
	return dot(*this);
}
template<size_t t_size>
const float Vector<t_size>::length() const {
	return sqrtf(lengthSquared());
}
template<size_t t_size>
const Vector<t_size> Vector<t_size>::normal() const {
	const float len = length();
	if (len <= 0.0f)
		return Vector<t_size>();
	return *this / len;
}
template<size_t t_size>
template<size_t, typename>
const Vector<t_size> Vector<t_size>::cross(const Vector<t_size>& other) const {
	return Vector<t_size>(
		(y() * other.z()) - (z() * other.y()),
		(z() * other.x()) - (x() * other.z()),
		(x() * other.y()) - (y() * other.x())
	);
}

#define VECTOR_OPERATOR(opSym)                                                               \
	template<size_t t_size>                                                                  \
	const Vector<t_size> Vector<t_size>::operator opSym(const Vector<t_size>& other) const { \
		Vector<t_size> result = *this;                                                       \
		for (size_t i = 0; i < t_size; i++) {                                                \
			result.m_data[i] = result.m_data[i] opSym other.m_data[i];                       \
		}                                                                                    \
		return result;                                                                       \
	}                                                                                        \
	template<size_t t_size>                                                                  \
	const Vector<t_size>& Vector<t_size>::operator opSym## = (const Vector<t_size>& other) { \
		for (size_t i = 0; i < t_size; i++) {                                                \
			m_data[i] = m_data[i] opSym other.m_data[i];                                     \
		}                                                                                    \
		return *this;                                                                        \
	}                                                                                        \
	template<size_t t_size>                                                                  \
	const Vector<t_size> Vector<t_size>::operator opSym(const float scalar) const {          \
		Vector<t_size> result = *this;                                                       \
		for (size_t i = 0; i < t_size; i++) {                                                \
			result.m_data[i] = result.m_data[i] opSym scalar;                                \
		}                                                                                    \
		return result;                                                                       \
	}                                                                                        \
	template<size_t t_size>                                                                  \
	const Vector<t_size>& Vector<t_size>::operator opSym## = (const float scalar) {          \
		for (size_t i = 0; i < t_size; i++) {                                                \
			m_data[i] = m_data[i] opSym scalar;                                              \
		}                                                                                    \
		return *this;                                                                        \
	}

VECTOR_OPERATOR(+);
VECTOR_OPERATOR(-);
VECTOR_OPERATOR(*);
VECTOR_OPERATOR(/);
#undef VECTOR_OPERATOR

template<size_t t_size>
const Vector<t_size> Vector<t_size>::operator-() {
	Vector<t_size> result = *this;
	for (size_t i = 0; i < t_size; i++) {
		result.m_data[i] = -result.m_data[i];
	}
	return result;
}

template<size_t t_size>
const bool Vector<t_size>::operator==(const Vector<t_size>& other) const {
	for (size_t i = 0; i < t_size; i++) {
		if (m_data[i] != other.m_data[i]) {
			return false;
		}
	}
	return true;
}
template<size_t t_size>
const bool Vector<t_size>::operator<(const Vector<t_size>& other) const {
	for (size_t i = 0; i < t_size; i++) {
		if (m_data[i] != other.m_data[i]) {
			return m_data[i] < other.m_data[i];
		}
	}
	return false;
}
}