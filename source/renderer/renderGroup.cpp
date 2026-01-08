#include <WR3CK/renderer/renderGroup.hpp>

namespace WR3CK::Internal
{
RenderGroup::RenderGroup(const Shader& shader, const Mesh& mesh) :
	m_shader(shader), m_mesh(mesh),
	m_instanceData(nullptr),
	m_instanceCount(0), m_instanceCapacity(0),
	m_vao(0), m_ivbo(0),
	m_modified(false) {
	setupVao();
}
RenderGroup::~RenderGroup() {
	WR3CK_CLEANUP(m_instanceData, free(m_instanceData));
	WR3CK_CLEANUP_GL(m_vao, glDeleteVertexArrays(1, &m_vao));
	WR3CK_CLEANUP_GL(m_ivbo, glDeleteBuffers(1, &m_ivbo));
}

void RenderGroup::addInstance(const RenderInstance& instanceData) {
	const ShaderData& shader = m_shader.get();
	if (shader.attributeInstanceTotalSize() <= 0)
		return;
	resize(m_instanceCount + 1);

	const auto& attributes = shader.attributes();
	for (auto it : instanceData.data()) {
		auto attribute = attributes.find(it.first);
		if (attribute == attributes.end() || attribute->second.m_isStatic || attribute->second.m_glType != it.second.glType())
			continue;

		const size_t dataSize = Internal::GL::getTypeByteSize(it.second.glType());
		void* const dataTarget = reinterpret_cast<void*>(
			reinterpret_cast<intptr_t>(m_instanceData) +
			(m_instanceCount * shader.attributeInstanceTotalSize()) +
			attribute->second.m_dataOffset
		);

		if (memcmp(dataTarget, it.second.data(), dataSize) == 0)
			continue;
		memcpy(dataTarget, it.second.data(), dataSize);

		m_modified = true;
	}

	m_instanceCount++;
}
void RenderGroup::draw() {
	const ShaderData& shader = m_shader.get();
	const MeshData& mesh = m_mesh.get();

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
void RenderGroup::clear() {
	m_instanceCount = 0;
}

void RenderGroup::setupVao() {
	glGenBuffers(1, &m_ivbo);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	const ShaderData& shader = m_shader.get();
	const MeshData& mesh = m_mesh.get();

	for (auto it : shader.attributes()) {
		const bool isStatic = it.second.m_isStatic;
		glBindBuffer(GL_ARRAY_BUFFER, isStatic ? mesh.vbo() : m_ivbo);

		glVertexAttribPointer(
			it.second.m_location,
			Internal::GL::getTypeElementCount(it.second.m_glType),
			Internal::GL::getTypeBase(it.second.m_glType),
			GL_FALSE,
			isStatic ? mesh.dataStride() : shader.attributeInstanceTotalSize(),
			(void*)(isStatic ? mesh.getAttributeOffset(it.first) : it.second.m_dataOffset)
		);
		glVertexAttribDivisor(it.second.m_location, isStatic ? 0 : 1);
		glEnableVertexAttribArray(it.second.m_location);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo());

	glBindVertexArray(0);
}
void RenderGroup::resize(const size_t size) {
	if (size <= m_instanceCapacity)
		return;

	if (m_instanceCapacity <= 0)
		m_instanceCapacity = 4;
	while (m_instanceCapacity < size)
		m_instanceCapacity *= 2;

	const ShaderData& shader = m_shader.get();
	void* temp = realloc(m_instanceData, m_instanceCapacity * shader.attributeInstanceTotalSize());
	WR3CK_ASSERT(temp != nullptr, "Failed to resize RenderGroup buffer.");
	m_instanceData = temp;
}
}
