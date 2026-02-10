#include <WR3CK/asset/plaintext.hpp>

namespace WR3CK
{
PlaintextData::PlaintextData() : PlaintextData("") {}
PlaintextData::PlaintextData(const std::string& data) : m_data(data) {}

PlaintextData& PlaintextData::operator=(const std::string& data) {
	m_data = data;
}
};
