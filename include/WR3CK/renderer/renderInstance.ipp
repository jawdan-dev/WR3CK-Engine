#include "renderInstance.hpp"

namespace WR3CK
{
template<typename T>
void RenderInstance::setData(const std::string& attributeName, const T& data) {
	auto it = m_data.find(attributeName);
	if (it != m_data.end()) {
		m_data.erase(it);
	}

	m_data.emplace(
		std::piecewise_construct,
		std::forward_as_tuple(attributeName),
		std::forward_as_tuple(data)
	);
}
}