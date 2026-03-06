#pragma once
#include <WR3CK/asset/assetHandle.hpp>

namespace WR3CK
{
class PlaintextData : public AssetBaseData {
public:
	PlaintextData();
	PlaintextData(const std::string& data);
	PlaintextData(const void* const data, const size_t dataSize);
	PlaintextData(const PlaintextData& other) = default;
	~PlaintextData();

	WR3CK_GETTER_MUT(std::string, string, std::string(reinterpret_cast<char*>(m_data)));
	WR3CK_GETTER_CONST(void* const, data, m_data);

	PlaintextData& operator=(const PlaintextData& other) = default;
	PlaintextData& operator=(const std::string& data);

private:
	void* m_data;
	size_t m_dataSize;
};
typedef AssetHandle<PlaintextData> PlaintextHandle;
}
