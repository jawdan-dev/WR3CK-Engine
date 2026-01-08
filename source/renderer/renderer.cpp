#include <WR3CK/renderer/renderer.hpp>

namespace WR3CK
{
RendererData::RendererData() :
	m_renderInstances(),
	m_viewProjection() {}
RendererData::~RendererData() {}

void RendererData::setViewProjection(const Matrix4& viewProjection) {
	m_viewProjection = viewProjection;
}

void RendererData::draw(const Mesh& mesh, const Shader& shader, const RenderUniformBuffer& uniformBuffer) {
	draw(mesh, shader, RenderInstance(), uniformBuffer);
}
void RendererData::draw(const Mesh& mesh, const Shader& shader, const RenderInstance& instance, const RenderUniformBuffer& uniformBuffer) {
	const std::tuple<Shader, Mesh, RenderUniformBuffer> key =
		std::make_tuple(shader, mesh, uniformBuffer);

	auto it = m_renderInstances.find(key);
	if (it == m_renderInstances.end()) {
		const auto emplaceResult = m_renderInstances.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(key),
			std::forward_as_tuple(shader, mesh)
		);
		it = emplaceResult.first;
	}

	it->second.addInstance(instance);
}
void RendererData::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& it : m_renderInstances) {
		const Shader shaderHandle = std::get<0>(it.first);
		const ShaderData& shader = shaderHandle.get();
		const RenderUniformBuffer& uniformBuffer = std::get<2>(it.first);

		glUseProgram(shader.program());
		uniformBuffer.bindAll(shader);

		auto viewMatrixIt = shader.uniforms().find("u_viewProjection");
		if (viewMatrixIt != shader.uniforms().end()) {
			glUniformMatrix4fv(
				viewMatrixIt->second.m_location,
				1, GL_FALSE, m_viewProjection.data()
			);
		}

		it.second.draw();
	}

	glBindVertexArray(0);
	glUseProgram(0);
}
void RendererData::clear() {
	auto it = m_renderInstances.begin();
	while (it != m_renderInstances.end()) {
		if (it->second.instanceCount() <= 0) {
			it = m_renderInstances.erase(it);
			continue;
		}
		it->second.clear();
		it++;
	}
}

std::map<std::tuple<Shader, Mesh, RenderUniformBuffer>, Internal::RenderGroup> m_renderInstances;
Matrix4 m_viewProjection;
}