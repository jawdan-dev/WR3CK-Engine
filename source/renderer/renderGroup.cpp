#include <WR3CK/renderer/renderGroup.hpp>

namespace WR3CK::Internal
{
RenderGroupData::RenderGroupData(const ShaderHandle& shaderHandle, const MeshHandle& meshHandle) :
	m_shaderHandle(shaderHandle), m_meshHandle(meshHandle),
	m_instanceData(nullptr),
	m_instanceCount(0), m_instanceCapacity(0),
	m_staticInstances(), m_staticCounter(0),
	m_vao(0), m_ivbo(0),
	m_modified(false) {
	setupVao();
}
RenderGroupData::~RenderGroupData() {
	WR3CK_CLEANUP(m_instanceData, free(m_instanceData));
	WR3CK_CLEANUP_GL(m_vao, glDeleteVertexArrays(1, &m_vao));
	WR3CK_CLEANUP_GL(m_ivbo, glDeleteBuffers(1, &m_ivbo));
}

void RenderGroupData::addInstance(const RenderInstance& instanceData) {
	const ShaderData& shader = m_shaderHandle.get();
	if (shader.attributeInstanceTotalSize() <= 0)
		return;

	void* const data = insertInstance(m_instanceCount);
	const auto& attributes = shader.attributes();
	for (auto it : instanceData.data()) {
		auto attribute = attributes.find(it.first);
		if (attribute == attributes.end() || attribute->second.m_isStatic || attribute->second.m_glType != it.second.glType())
			continue;

		const size_t dataSize = Internal::GL::getTypeByteSize(it.second.glType());
		void* const dataTarget = reinterpret_cast<void*>(reinterpret_cast<intptr_t>(data) + attribute->second.m_dataOffset);

		if (memcmp(dataTarget, it.second.data(), dataSize) == 0)
			continue;
		memcpy(dataTarget, it.second.data(), dataSize);

		m_modified = true;
	}
}
const RenderGroupData::statickey_t RenderGroupData::addStaticInstance(const StaticRenderInstance& instanceData) {
	const ShaderData& shader = m_shaderHandle.get();
	WR3CK_ASSERT(shader.attributeInstanceTotalSize() > 0);

	const size_t staticIndex = m_staticInstances.size();
	void* const data = insertInstance(staticIndex);
	const auto& attributes = shader.attributes();
	for (auto it : instanceData.data()) {
		auto attribute = attributes.find(it.first);
		if (attribute == attributes.end() || attribute->second.m_isStatic || attribute->second.m_glType != it.second.glType())
			continue;

		const size_t dataSize = Internal::GL::getTypeByteSize(it.second.glType());
		void* const dataTarget = reinterpret_cast<void*>(reinterpret_cast<intptr_t>(data) + attribute->second.m_dataOffset);

		if (memcmp(dataTarget, it.second.data(), dataSize) == 0)
			continue;
		memcpy(dataTarget, it.second.data(), dataSize);

		m_modified = true;
	}

	statickey_t staticKey = m_staticCounter++;
	while (m_staticInstances.find(staticKey) != m_staticInstances.end())
		staticKey = m_staticCounter++;
	m_staticInstances.emplace(staticKey, staticIndex);

	return staticKey;
}
void RenderGroupData::updateStaticInstance(const statickey_t staticKey, const StaticRenderInstance& instanceData) {
	const ShaderData& shader = m_shaderHandle.get();
	if (shader.attributeInstanceTotalSize() <= 0)
		return;

	auto it = m_staticInstances.find(staticKey);
	if (it == m_staticInstances.end()) {
		WR3CK_LOG_WARNING("Failed to find static StaticRenderIstance \"%u\" in RenderGroup.", staticKey);
		return;
	}
	const size_t staticIndex = it->second;

	void* const data = getInstance(staticIndex);
	const auto& attributes = shader.attributes();
	for (auto it : instanceData.data()) {
		auto attribute = attributes.find(it.first);
		if (attribute == attributes.end() || attribute->second.m_isStatic || attribute->second.m_glType != it.second.glType())
			continue;

		const size_t dataSize = Internal::GL::getTypeByteSize(it.second.glType());
		void* const dataTarget = reinterpret_cast<void*>(reinterpret_cast<intptr_t>(data) + attribute->second.m_dataOffset);

		if (memcmp(dataTarget, it.second.data(), dataSize) == 0)
			continue;
		memcpy(dataTarget, it.second.data(), dataSize);

		m_modified = true;
	}
}
void RenderGroupData::removeStaticInstance(const statickey_t staticKey) {
	auto it = m_staticInstances.find(staticKey);
	if (it == m_staticInstances.end()) {
		WR3CK_LOG_WARNING("Failed to find static StaticRenderIstance \"%u\" in RenderGroup.", staticKey);
		return;
	}
	const size_t removeIndex = it->second;
	removeInstance(removeIndex);

	m_staticInstances.erase(it);
	for (auto& it : m_staticInstances) {
		if (it.second <= removeIndex)
			continue;
		it.second--;
	}
}

void RenderGroupData::render() {
	const ShaderData& shader = m_shaderHandle.get();
	const MeshData& mesh = m_meshHandle.get();

	if (m_modified) {
		// TODO: Named functions?
		glBindBuffer(GL_ARRAY_BUFFER, m_ivbo);
		glBufferData(GL_ARRAY_BUFFER, m_instanceCapacity * shader.attributeInstanceTotalSize(), m_instanceData, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_modified = false;
	}

	if (mesh.vbo() == 0)
		return;

	const bool instanced = shader.attributeInstanceTotalSize() > 0;

	glBindVertexArray(m_vao);
	if (mesh.ebo() != 0) {
		if (instanced) {
			glDrawElementsInstanced(GL_TRIANGLES, mesh.drawCount(), GL_UNSIGNED_SHORT, nullptr, m_instanceCount);
		} else {
			glDrawElements(GL_TRIANGLES, mesh.drawCount(), GL_UNSIGNED_SHORT, nullptr);
		}
	} else if (instanced) {
		glDrawArraysInstanced(GL_TRIANGLES, 0, mesh.drawCount(), m_instanceCount);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, mesh.drawCount());
	}
}
void RenderGroupData::clear() {
	m_instanceCount = m_staticInstances.size();
}

void RenderGroupData::setupVao() {
	glGenBuffers(1, &m_ivbo);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	const ShaderData& shader = m_shaderHandle.get();
	const MeshData& mesh = m_meshHandle.get();

	for (auto it : shader.attributes()) {
		const bool isStatic = it.second.m_isStatic;
		glBindBuffer(GL_ARRAY_BUFFER, isStatic ? mesh.vbo() : m_ivbo);

		switch (Internal::GL::getTypeBase(it.second.m_glType)) {
			default:
				glVertexAttribPointer(
					it.second.m_location,
					Internal::GL::getTypeElementCount(it.second.m_glType),
					Internal::GL::getTypeBase(it.second.m_glType),
					GL_FALSE,
					isStatic ? mesh.dataStride() : shader.attributeInstanceTotalSize(),
					(void*)(isStatic ? mesh.getAttributeOffset(it.first) : it.second.m_dataOffset)
				);
				break;

			case GL_BYTE:
			case GL_UNSIGNED_BYTE:
			case GL_SHORT:
			case GL_UNSIGNED_SHORT:
			case GL_INT:
			case GL_UNSIGNED_INT:
				glVertexAttribIPointer(
					it.second.m_location,
					Internal::GL::getTypeElementCount(it.second.m_glType),
					Internal::GL::getTypeBase(it.second.m_glType),
					isStatic ? mesh.dataStride() : shader.attributeInstanceTotalSize(),
					(void*)(isStatic ? mesh.getAttributeOffset(it.first) : it.second.m_dataOffset)
				);
				break;

			case GL_DOUBLE:
				glVertexAttribLPointer(
					it.second.m_location,
					Internal::GL::getTypeElementCount(it.second.m_glType),
					Internal::GL::getTypeBase(it.second.m_glType),
					isStatic ? mesh.dataStride() : shader.attributeInstanceTotalSize(),
					(void*)(isStatic ? mesh.getAttributeOffset(it.first) : it.second.m_dataOffset)
				);
				break;
		}
		glVertexAttribDivisor(it.second.m_location, isStatic ? 0 : 1);
		glEnableVertexAttribArray(it.second.m_location);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo());

	glBindVertexArray(0);
}
void RenderGroupData::resize(const size_t size) {
	if (size <= m_instanceCapacity)
		return;

	if (m_instanceCapacity <= 0)
		m_instanceCapacity = 4;
	while (m_instanceCapacity < size)
		m_instanceCapacity *= 2;

	const ShaderData& shader = m_shaderHandle.get();
	void* temp = realloc(m_instanceData, m_instanceCapacity * shader.attributeInstanceTotalSize());
	WR3CK_ASSERT(temp != nullptr, "Failed to resize RenderGroup buffer.");
	m_instanceData = temp;
}
void* RenderGroupData::getInstance(const size_t index) {
	WR3CK_ASSERT(index < m_instanceCapacity);
	return reinterpret_cast<void*>(
		reinterpret_cast<intptr_t>(m_instanceData) +
		(index * m_shaderHandle.get().attributeInstanceTotalSize())
	);
}
void RenderGroupData::removeInstance(const size_t _index) {
	WR3CK_ASSERT(_index < m_instanceCount);
	const size_t index = __min(m_instanceCount, _index + 1);
	const size_t moveCount = m_instanceCount - index;

	if (moveCount > 0) {
		memmove(
			getInstance(index - 1),
			getInstance(index),
			moveCount * m_shaderHandle.get().attributeInstanceTotalSize()
		);
		m_modified = true;
	}

	m_instanceCount--;
}
void* RenderGroupData::insertInstance(const size_t _index) {
	const size_t index = __min(m_instanceCount, _index);
	resize(m_instanceCount + 1);

	const size_t moveCount = m_instanceCount - index;
	if (moveCount > 0) {
		memmove(
			getInstance(index + 1),
			getInstance(index),
			moveCount * m_shaderHandle.get().attributeInstanceTotalSize()
		);
		m_modified = true;
	}

	m_instanceCount++;
	return getInstance(index);
}
}
