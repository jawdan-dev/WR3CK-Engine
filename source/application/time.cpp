#include <WR3CK/application/time.hpp>

namespace WR3CK
{
Time::Time() :
	m_now(0), m_last(0),
	m_delta(0) {}

const void Time::updateTime(const float timeNow) {
	m_last = m_now;
	m_now = timeNow;
	m_delta = m_now - m_last;

	if (m_delta > 0.1f) {
		m_delta = 0.1f;
	}
}
}