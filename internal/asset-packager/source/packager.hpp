#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/file/assetPackage.hpp>
#include <fileData.hpp>

namespace WR3CK
{
class Packager {
public:
	Packager();
	Packager(const Packager& other) = default;
	~Packager() = default;

	WR3CK_GETTER_CONST(ARG(std::map<std::string, FileData>&), fileDataMap, m_fileDataMap)

	void addPath(const std::string& path, const size_t prefixLength = 0);

	void saveToFile(const std::string& path);

private:
	std::map<std::string, FileData> m_fileDataMap;
};
}