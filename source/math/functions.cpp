#include <WR3CK/math/math.hpp>

namespace WR3CK
{
const float Math::abs(const float x) {
	return fabsf(x);
}
const float Math::remap(const float x, const float xMin, const float xMax, const float tMin, const float tMax) {
	return (((x - xMin) / (xMax - xMin)) * (tMax - tMin)) + tMin;
}
const float Math::min(const float a, const float b) {
	return __min(a, b);
}
const float Math::max(const float a, const float b) {
	return __max(a, b);
}
const float Math::clamp(const float x, const float min, const float max) {
	return Math::min(Math::max(x, min), max);
}
const float Math::moveTowards(const float x, const float target, const float amount) {
	const float diff = target - x;

	const float absDiff = Math::abs(diff), absAmount = Math::abs(amount);
	if (absDiff <= absAmount)
		return target;

	return (diff < 0.0f) ? (x - absAmount) : (x + absAmount);
}
}