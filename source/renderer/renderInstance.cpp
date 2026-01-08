#include <WR3CK/renderer/renderInstance.hpp>

namespace WR3CK
{
RenderInstance::RenderInstance() : m_data() {}

const bool RenderInstance::operator<(const RenderInstance& other) const {
	auto leftIt = m_data.begin();
	auto rightIt = other.m_data.begin();

	while (leftIt != m_data.end() && rightIt != other.m_data.end()) {
		if (leftIt->first != rightIt->first)
			return leftIt->first < rightIt->first;

		if (leftIt->second != rightIt->second)
			return leftIt->second < rightIt->second;

		leftIt++;
		rightIt++;
	}

	if (leftIt == m_data.end())
		return rightIt != other.m_data.end();

	return false;
}
}