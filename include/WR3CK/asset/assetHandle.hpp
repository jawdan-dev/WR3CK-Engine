#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/asset/assetData.hpp>

namespace WR3CK
{
template<typename T, typename = std::enable_if_t<std::is_base_of_v<AssetData, T>>>
using AssetHandle = Handle<T>;

typedef AssetHandle<AssetData> Asset;
}