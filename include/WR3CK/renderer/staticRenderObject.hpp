#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/asset/assetHandle.hpp>
#include <WR3CK/renderer/renderGroup.hpp>

namespace WR3CK
{
class StaticRenderObjectData : public AssetBaseData {
public:
	StaticRenderObjectData(const Internal::RenderGroupHandle& renderGroupHandle, const Internal::RenderGroupData::statickey_t staticKey);
	StaticRenderObjectData(const StaticRenderObjectData& other) = delete;
	~StaticRenderObjectData();

	WR3CK_GETTER_CONST(Internal::RenderGroupHandle&, renderGroup, m_renderGroupHandle);
	WR3CK_GETTER_CONST(Internal::RenderGroupData::statickey_t, staticKey, m_staticKey);

private:
	Internal::RenderGroupHandle m_renderGroupHandle;
	Internal::RenderGroupData::statickey_t m_staticKey;
};
typedef AssetHandle<StaticRenderObjectData> StaticRenderObjectHandle;
}