#pragma once
#include <WR3CK/core/core.hpp>

namespace WR3CK
{
class Time {
public:
	Time();
	Time(const Time& other) = delete;
	~Time() = default;

	WR3CK_GETTER_CONST(float, delta, m_delta);
	WR3CK_GETTER_CONST(float, now, m_now);

	const void updateTime(const float timeNow);

private:
	float m_now, m_last;
	float m_delta;
};
}