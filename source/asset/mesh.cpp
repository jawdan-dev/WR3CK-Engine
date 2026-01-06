#include <WR3CK/asset/mesh.hpp>

namespace WR3CK
{
MeshData::MeshData() :
	m_data(), m_indices(),
	m_dataStride(0), m_drawCount(0),
	m_vbo(0), m_ebo(0),
	m_modified(false) {}
MeshData::~MeshData() {
	WR3CK_CLEANUP_GL(m_vbo, glDeleteBuffers(1, &m_vbo));
	WR3CK_CLEANUP_GL(m_ebo, glDeleteBuffers(1, &m_ebo));
}

const size_t MeshData::getAttributeOffset(const std::string& attributeName) const {
	size_t offset = 0;
	for (auto it : m_data) {
		const size_t dataSize = Internal::GL::getTypeByteSize(it.second.glType());
		if (it.first == attributeName)
			return offset;
		offset += dataSize;
	}
	return 0;
}

void MeshData::setIndices(const std::vector<index_t>& indices) {
	m_indices = indices;
	m_modified = true;
}

void MeshData::commit() {
	if (!m_modified)
		return;
	m_modified = false;

	m_drawCount = 0;
	if (m_data.size() > 0) {
		// Calculate stride.
		m_dataStride = 0;
		for (auto it : m_data) {
			m_dataStride += Internal::GL::getTypeByteSize(it.second.glType());
			m_drawCount = __max(m_drawCount, it.second.dataCount());
		}

		// Set data buffer.
		void* data = malloc(m_drawCount * m_dataStride);
		for (auto it : m_data) {
			const size_t dataSize = Internal::GL::getTypeByteSize(it.second.glType());
			const size_t offset = getAttributeOffset(it.first);

			for (size_t i = 0; i < it.second.dataCount() && i < m_drawCount; i++) {
				memcpy(
					reinterpret_cast<void*>(reinterpret_cast<intptr_t>(data) + (i * m_dataStride) + offset),
					reinterpret_cast<void*>(reinterpret_cast<intptr_t>(it.second.data()) + (i * dataSize)),
					dataSize
				);
			}
		}

		// Send to GPU.
		if (m_vbo == 0) {
			glGenBuffers(1, &m_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_drawCount * m_dataStride, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Cleanup.
		free(data);
	} else {
		WR3CK_CLEANUP_GL(m_vbo, glDeleteBuffers(1, &m_vbo));
	}

	if (m_indices.size() > 0) {
		if (!m_ebo) {
			glGenBuffers(1, &m_ebo);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(index_t), m_indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_drawCount = m_indices.size();
	} else {
		WR3CK_CLEANUP_GL(m_ebo, glDeleteBuffers(1, &m_ebo));
	}
}
}