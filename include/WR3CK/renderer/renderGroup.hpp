#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/asset/mesh.hpp>
#include <WR3CK/asset/shader.hpp>
#include <WR3CK/renderer/renderInstance.hpp>
#include <WR3CK/renderer/staticRenderInstance.hpp>

namespace WR3CK::Internal
{
class RenderGroupData : public AssetBaseData {
public:
	typedef uint32_t statickey_t;

	RenderGroupData(const ShaderHandle& shaderHandle, const MeshHandle& meshHandle);
	RenderGroupData(const RenderGroupData& other) = delete;
	~RenderGroupData();

	WR3CK_GETTER_CONST(ShaderHandle, shader, m_shaderHandle);
	WR3CK_GETTER_CONST(MeshHandle, mesh, m_meshHandle);
	WR3CK_GETTER_CONST(size_t, instanceCount, m_instanceCount);

	void addInstance(const RenderInstance& instanceData);
	const statickey_t addStaticInstance(const StaticRenderInstance& instanceData);
	void updateStaticInstance(const statickey_t staticKey, const StaticRenderInstance& instanceData);
	void removeStaticInstance(const statickey_t staticKey);

	void render();
	void clear();

private:
	void setupVao();
	void resize(const size_t size);
	void* getInstance(const size_t index);
	void removeInstance(const size_t index);
	void* insertInstance(const size_t index = SIZE_MAX);

	ShaderHandle m_shaderHandle;
	MeshHandle m_meshHandle;
	void* m_instanceData;
	size_t m_instanceCount, m_instanceCapacity;
	std::map<statickey_t, size_t> m_staticInstances;
	statickey_t m_staticCounter;
	GLuint m_vao, m_ivbo;
	bool m_modified;
};
typedef AssetHandle<RenderGroupData> RenderGroupHandle;
}
