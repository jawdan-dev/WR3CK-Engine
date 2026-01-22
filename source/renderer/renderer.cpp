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

void RendererData::draw(const MeshHandle& meshHandle, const ShaderHandle& shaderHandle, const RenderUniformBuffer& uniformBuffer) {
	draw(meshHandle, shaderHandle, RenderInstance(), uniformBuffer);
}
void RendererData::draw(const MeshHandle& meshHandle, const ShaderHandle& shaderHandle, const RenderInstance& instance, const RenderUniformBuffer& uniformBuffer) {
	getRenderGroup(meshHandle, shaderHandle, uniformBuffer).get().addInstance(instance);
}
const StaticRenderObjectHandle RendererData::draw(const MeshHandle& meshHandle, const ShaderHandle& shaderHandle, const StaticRenderInstance& instance, const RenderUniformBuffer& uniformBuffer) {
	Internal::RenderGroupHandle& renderGroupHandle = getRenderGroup(meshHandle, shaderHandle, uniformBuffer);
	Internal::RenderGroupData& renderGroup = renderGroupHandle.get();
	Internal::RenderGroupData::statickey_t staticKey = renderGroup.addStaticInstance(instance);
	return StaticRenderObjectHandle(renderGroupHandle, staticKey);
}
void RendererData::draw(StaticRenderObjectHandle& staticRenderObjectHandle, const StaticRenderInstance& instance) {
	StaticRenderObjectData& staticRenderObject = staticRenderObjectHandle.get();
	Internal::RenderGroupHandle renderGroupHandle(staticRenderObject.renderGroup());
	Internal::RenderGroupData& renderGroup = renderGroupHandle.get();
	renderGroup.updateStaticInstance(staticRenderObject.staticKey(), instance);
}

void RendererData::render(FrameBufferHandle frameBufferHandle) {
	return render(frameBufferHandle.get());
}
void RendererData::render(FrameBufferData& frameBuffer) {
	frameBuffer.clear();
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.frameBuffer());

	for (auto& shaderMeshIt : m_renderInstances) {
		const ShaderHandle& shaderHandle = shaderMeshIt.first.first;
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

			it.second.get().render();
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
			Internal::RenderGroupData& renderGroup = it->second.get();
			if (renderGroup.instanceCount() <= 0) {
				it = shaderMeshIt->second.erase(it);
				continue;
			}
			renderGroup.clear();
			it++;
		}
		if (shaderMeshIt->second.size() <= 0) {
			shaderMeshIt = m_renderInstances.erase(shaderMeshIt);
			continue;
		}
		shaderMeshIt++;
	}
}

Internal::RenderGroupHandle& RendererData::getRenderGroup(const MeshHandle& meshHandle, const ShaderHandle& shaderHandle, const RenderUniformBuffer& uniformBuffer) {
	const std::pair<ShaderHandle, MeshHandle> key(shaderHandle, meshHandle);

	auto shaderMeshIt = m_renderInstances.find(key);
	if (shaderMeshIt == m_renderInstances.end()) {
		const auto emplaceResult = m_renderInstances.emplace(key, std::map<RenderUniformBuffer, Internal::RenderGroupHandle>());
		shaderMeshIt = emplaceResult.first;
	}

	auto it = shaderMeshIt->second.find(uniformBuffer);
	if (it == shaderMeshIt->second.end()) {
		const auto emplaceResult = shaderMeshIt->second.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(uniformBuffer),
			std::forward_as_tuple(shaderHandle, meshHandle)
		);
		it = emplaceResult.first;
	}

	return it->second;
}
}