#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/asset/assetBaseData.hpp>

namespace WR3CK
{
template<typename T, typename = std::enable_if_t<std::is_base_of_v<AssetBaseData, T>>>
using AssetHandle = Handle<T>;

typedef AssetHandle<AssetBaseData> AssetBaseHandle;
}