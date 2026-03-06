#include <WR3CK/asset/plaintext.hpp>

namespace WR3CK
{
PlaintextData::PlaintextData() : PlaintextData(nullptr, 0) {}
PlaintextData::PlaintextData(const std::string& data) : PlaintextData(data.c_str(), data.size()) {}
PlaintextData::PlaintextData(const void* const data, const size_t dataSize) :
	m_data(nullptr), m_dataSize(dataSize) {
	if (data == nullptr || dataSize <= 0)
		return;
	m_data = malloc(m_dataSize + 1);
	memcpy(m_data, data, m_dataSize);
	reinterpret_cast<char*>(m_data)[m_dataSize] = '\0';
}
PlaintextData::~PlaintextData() {
	WR3CK_CLEANUP(m_data, free(m_data); m_dataSize = 0);
}

PlaintextData& PlaintextData::operator=(const std::string& data) {
	PlaintextData::~PlaintextData();
	m_dataSize = data.size();
	if (m_dataSize <= 0)
		return *this;
	m_data = malloc(m_dataSize + 1);
	memcpy(m_data, data.c_str(), m_dataSize);
	reinterpret_cast<char*>(m_data)[m_dataSize] = '\0';
	return *this;
}
};
