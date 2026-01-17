#pragma once

namespace WR3CK::Math
{
const float abs(const float x);
const float remap(const float x, const float xMin, const float xMax, const float tMin, const float tMax);
const float min(const float a, const float b);
const float max(const float a, const float b);
const float clamp(const float x, const float min, const float max);
const float moveTowards(const float x, const float target, const float amount);
}