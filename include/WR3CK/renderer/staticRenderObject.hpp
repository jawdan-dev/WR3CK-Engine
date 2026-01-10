#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/asset/assetHandle.hpp>
#include <WR3CK/renderer/renderGroup.hpp>

namespace WR3CK
{
class StaticRenderObjectData : public AssetData {
public:
	StaticRenderObjectData(const Internal::RenderGroup& renderGroup, const Internal::RenderGroupData::statickey_t staticKey);
	StaticRenderObjectData(const StaticRenderObjectData& other) = delete;
	~StaticRenderObjectData();

	WR3CK_GETTER_CONST(Internal::RenderGroup&, renderGroup, m_renderGroup);
	WR3CK_GETTER_CONST(Internal::RenderGroupData::statickey_t, staticKey, m_staticKey);

private:
	Internal::RenderGroup m_renderGroup;
	Internal::RenderGroupData::statickey_t m_staticKey;
};
typedef AssetHandle<StaticRenderObjectData> StaticRenderObject;
}