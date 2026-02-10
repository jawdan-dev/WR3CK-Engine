#pragma once
#include <WR3CK/asset/assetHandle.hpp>

namespace WR3CK
{
class PlaintextData : public AssetBaseData {
public:
	PlaintextData();
	PlaintextData(const std::string& data);
	PlaintextData(const PlaintextData& other) = default;
	~PlaintextData() = default;

	WR3CK_GETTER_MUT(std::string&, string, m_data);
	WR3CK_GETTER_CONST(char* const, c_str, m_data.c_str());

	PlaintextData& operator=(const PlaintextData& other) = default;
	PlaintextData& operator=(const std::string& data);

private:
	std::string m_data;
};
typedef AssetHandle<PlaintextData> PlaintextHandle;
}
