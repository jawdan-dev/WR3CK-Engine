#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/asset/mesh.hpp>
#include <WR3CK/asset/shader.hpp>
#include <WR3CK/renderer/renderInstance.hpp>
#include <WR3CK/renderer/staticRenderInstance.hpp>

namespace WR3CK::Internal
{
class RenderGroupData : public AssetData {
public:
	typedef uint32_t statickey_t;

	RenderGroupData(const Shader& shader, const Mesh& mesh);
	RenderGroupData(const RenderGroupData& other) = delete;
	~RenderGroupData();

	WR3CK_GETTER_CONST(Shader, shader, m_shader);
	WR3CK_GETTER_CONST(Mesh, mesh, m_mesh);
	WR3CK_GETTER_CONST(size_t, instanceCount, m_instanceCount);

	void addInstance(const RenderInstance& instanceData);
	const statickey_t addStaticInstance(const StaticRenderInstance& instanceData);
	void updateStaticInstance(const statickey_t staticReference, const StaticRenderInstance& instanceData);
	void removeStaticInstance(const statickey_t staticReference);

	void render();
	void clear();

private:
	void setupVao();
	void resize(const size_t size);
	void* getInstance(const size_t index);
	void removeInstance(const size_t index);
	void* insertInstance(const size_t index = SIZE_MAX);

	Shader m_shader;
	Mesh m_mesh;
	void* m_instanceData;
	size_t m_instanceCount, m_instanceCapacity;
	std::map<statickey_t, size_t> m_staticInstances;
	statickey_t m_staticCounter;
	GLuint m_vao, m_ivbo;
	bool m_modified;
};
typedef AssetHandle<RenderGroupData> RenderGroup;
}
