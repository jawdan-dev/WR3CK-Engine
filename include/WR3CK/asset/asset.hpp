#pragma once
#include <WR3CK/core/core.hpp>

namespace WR3CK
{
class AssetData {};

template<typename T, typename = std::enable_if_t<std::is_base_of_v<AssetData, T>>>
using AssetHandle = Handle<T>;

typedef AssetHandle<AssetData> Asset;
}