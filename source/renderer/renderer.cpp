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
	const std::pair<Shader, Mesh> key(shader, mesh);

	auto shaderMeshIt = m_renderInstances.find(key);
	if (shaderMeshIt == m_renderInstances.end()) {
		const auto emplaceResult = m_renderInstances.emplace(key, std::map<RenderUniformBuffer, Internal::RenderGroup>());
		shaderMeshIt = emplaceResult.first;
	}

	auto it = shaderMeshIt->second.find(uniformBuffer);
	if (it == shaderMeshIt->second.end()) {
		const auto emplaceResult = shaderMeshIt->second.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(uniformBuffer),
			std::forward_as_tuple(shader, mesh)
		);
		it = emplaceResult.first;
	}

	it->second.addInstance(instance);
}
void RendererData::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& shaderMeshIt : m_renderInstances) {
		const Shader shaderHandle = shaderMeshIt.first.first;
		const ShaderData& shader = shaderHandle.get();

		glUseProgram(shader.program());

		auto viewMatrixIt = shader.uniforms().find("u_viewProjection");
		if (viewMatrixIt != shader.uniforms().end()) {
			glUniformMatrix4fv(
				viewMatrixIt->second.m_location,
				1, GL_FALSE, m_viewProjection.data()
			);
		}

		for (auto& it : shaderMeshIt.second) {
			const RenderUniformBuffer& uniformBuffer = it.first;

			// TODO: Add in-built variable protection. Uniform buffer will probably solve that!
			uniformBuffer.bindAll(shader);

			it.second.draw();
		}
	}

	glBindVertexArray(0);
	glUseProgram(0);
}
void RendererData::clear() {
	auto shaderMeshIt = m_renderInstances.begin();
	while (shaderMeshIt != m_renderInstances.end()) {
		auto it = shaderMeshIt->second.begin();
		while (it != shaderMeshIt->second.end()) {
			if (it->second.instanceCount() <= 0) {
				it = shaderMeshIt->second.erase(it);
				continue;
			}
			it->second.clear();
			it++;
		}
		if (shaderMeshIt->second.size() <= 0) {
			shaderMeshIt = m_renderInstances.erase(shaderMeshIt);
			continue;
		}
		shaderMeshIt++;
	}
}

std::map<std::tuple<Shader, Mesh, RenderUniformBuffer>, Internal::RenderGroup> m_renderInstances;
Matrix4 m_viewProjection;
}