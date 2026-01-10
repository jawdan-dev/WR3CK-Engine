#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/asset/shader.hpp>
#include <WR3CK/asset/mesh.hpp>
#include <WR3CK/renderer/renderUniformBuffer.hpp>
#include <WR3CK/renderer/renderGroup.hpp>
#include <WR3CK/renderer/staticRenderObject.hpp>

namespace WR3CK
{
class RendererData : public AssetData {
public:
	RendererData();
	RendererData(const RendererData& other) = delete;
	~RendererData();

	WR3CK_GETTER_CONST(Matrix4&, viewProjection, m_viewProjection);

	void setViewProjection(const Matrix4& viewProjection);

	void draw(const Mesh& mesh, const Shader& shader, const RenderUniformBuffer& uniformBuffer = RenderUniformBuffer());
	void draw(const Mesh& mesh, const Shader& shader, const RenderInstance& instance, const RenderUniformBuffer& uniformBuffer = RenderUniformBuffer());
	const StaticRenderObject draw(const Mesh& mesh, const Shader& shader, const StaticRenderInstance& instance, const RenderUniformBuffer& uniformBuffer = RenderUniformBuffer());
	void draw(StaticRenderObject& staticRender, const StaticRenderInstance& instance);
	void render();
	void clear();

private:
	Internal::RenderGroup& getRenderGroup(const Mesh& mesh, const Shader& shader, const RenderUniformBuffer& uniformBuffer);

	std::map<std::pair<Shader, Mesh>, std::map<RenderUniformBuffer, Internal::RenderGroup>> m_renderInstances;
	Matrix4 m_viewProjection;
};
typedef AssetHandle<RendererData> Renderer;
}