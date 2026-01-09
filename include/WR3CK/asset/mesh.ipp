#pragma once
#include "mesh.hpp"

namespace WR3CK
{
template<typename T>
void MeshData::setData(const std::string& attributeName, const std::vector<T>& data) {
	auto it = m_data.find(attributeName);
	if (it != m_data.end()) {
		it->second = data;
		return;
	}
	m_data.emplace(
		std::piecewise_construct,
		std::forward_as_tuple(attributeName),
		std::forward_as_tuple(data)
	);
}
}