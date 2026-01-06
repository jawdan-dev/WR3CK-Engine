#pragma once
#include "mesh.hpp"

namespace WR3CK
{
template<typename T>
void MeshData::setData(const std::string& attributeName, const std::vector<T>& data) {
	auto it = m_data.find(attributeName);
	if (it != m_data.end()) {
		m_data.erase(it);
	}

	const Internal::RenderData vertexData(data);
	m_data.emplace(attributeName, vertexData);
}
}