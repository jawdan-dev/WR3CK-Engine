#pragma once
#include <WR3CK/asset/assetHandle.hpp>

#include <WR3CK/renderer/renderData.hpp>

namespace WR3CK
{
class MeshData : public AssetData {
public:
	typedef uint16_t index_t;

	MeshData();
	MeshData(const MeshData& other) = delete;
	~MeshData();

	WR3CK_GETTER_CONST(ARG(std::map<std::string, Internal::RenderData>&), data, m_data);
	WR3CK_GETTER_CONST(std::vector<index_t>&, indices, m_indices);
	WR3CK_GETTER_CONST(size_t, dataStride, m_dataStride);
	WR3CK_GETTER_CONST(size_t, drawCount, m_drawCount);
	WR3CK_GETTER_CONST(GLuint, vbo, m_vbo);
	WR3CK_GETTER_CONST(GLuint, ebo, m_ebo);

	const size_t getAttributeOffset(const std::string& dataName) const;

	template<typename T>
	void setData(const std::string& attributeName, const std::vector<T>& data);
	void setIndices(const std::vector<index_t>& indices);

	void commit();

private:
	std::map<std::string, Internal::RenderData> m_data;
	std::vector<index_t> m_indices;
	size_t m_dataStride, m_drawCount;
	GLuint m_vbo, m_ebo;
	bool m_modified;
};
typedef AssetHandle<MeshData> Mesh;
}

#include "mesh.ipp"