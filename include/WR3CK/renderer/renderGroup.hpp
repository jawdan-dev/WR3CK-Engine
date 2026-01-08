#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/asset/mesh.hpp>
#include <WR3CK/asset/shader.hpp>
#include <WR3CK/renderer/renderInstance.hpp>

namespace WR3CK::Internal
{
class RenderGroup {
public:
	RenderGroup(const Shader& shader, const Mesh& mesh);
	RenderGroup(const RenderGroup& other) = delete;
	~RenderGroup();

	WR3CK_GETTER_CONST(Shader, shader, m_shader);
	WR3CK_GETTER_CONST(Mesh, mesh, m_mesh);
	WR3CK_GETTER_CONST(size_t, instanceCount, m_instanceCount);

	void addInstance(const RenderInstance& instanceData);
	void draw();
	void clear();

private:
	void setupVao();
	void resize(const size_t size);

	Shader m_shader;
	Mesh m_mesh;
	void* m_instanceData;
	size_t m_instanceCount, m_instanceCapacity;
	GLuint m_vao, m_ivbo;
	bool m_modified;
};
}
