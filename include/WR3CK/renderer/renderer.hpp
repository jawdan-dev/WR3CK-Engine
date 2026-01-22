#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/asset/shader.hpp>
#include <WR3CK/asset/mesh.hpp>
#include <WR3CK/asset/frameBuffer.hpp>
#include <WR3CK/renderer/renderUniformBuffer.hpp>
#include <WR3CK/renderer/renderGroup.hpp>
#include <WR3CK/renderer/staticRenderObject.hpp>

namespace WR3CK
{
class RendererData : public AssetBaseData {
public:
	RendererData();
	RendererData(const RendererData& other) = delete;
	~RendererData();

	WR3CK_GETTER_CONST(Matrix4&, viewProjection, m_viewProjection);

	void setViewProjection(const Matrix4& viewProjection);

	void draw(const MeshHandle& meshHandle, const ShaderHandle& shaderHandle, const RenderUniformBuffer& uniformBuffer = RenderUniformBuffer());
	void draw(const MeshHandle& meshHandle, const ShaderHandle& shaderHandle, const RenderInstance& instance, const RenderUniformBuffer& uniformBuffer = RenderUniformBuffer());
	const StaticRenderObjectHandle draw(const MeshHandle& meshHandle, const ShaderHandle& shaderHandle, const StaticRenderInstance& instance, const RenderUniformBuffer& uniformBuffer = RenderUniformBuffer());
	void draw(StaticRenderObjectHandle& staticRenderObjectHandle, const StaticRenderInstance& instance);
	void render(FrameBufferHandle frameBufferHandle);
	void render(FrameBufferData& frameBuffer);
	void clear();

private:
	Internal::RenderGroupHandle& getRenderGroup(const MeshHandle& meshHandle, const ShaderHandle& shaderHandle, const RenderUniformBuffer& uniformBuffer);

	std::map<std::pair<ShaderHandle, MeshHandle>, std::map<RenderUniformBuffer, Internal::RenderGroupHandle>> m_renderInstances;
	Matrix4 m_viewProjection;
};
typedef AssetHandle<RendererData> RendererHandle;
}