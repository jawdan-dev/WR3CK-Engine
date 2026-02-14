#include "packager.hpp"

#include <filesystem>

namespace WR3CK
{
Packager::Packager() : m_fileDataMap() {};

void Packager::addPath(const std::string& path, const size_t prefixLength) {
	WR3CK_ASSERT(std::filesystem::exists(path), "Asset path \"%s\" does not exist.", path.c_str());

	if (std::filesystem::is_directory(path)) {
		for (const auto it : std::filesystem::recursive_directory_iterator(path)) {
			if (it.is_directory())
				continue;

			const size_t localPrefixLength = (it.path().generic_string().size() - it.path().filename().generic_string().size()) - (path.size() + 1);
			addPath(it.path().generic_string(), prefixLength + localPrefixLength);
		}
		return;
	}

	if (!std::filesystem::is_regular_file(path)) {
		WR3CK_LOG_WARNING("Non-generic file types not supported.\n");
		return;
	}

	const std::filesystem::path filePath(path);
	const std::string& fileName = filePath.filename().generic_string();
	const std::string fileStoreName = path.substr(path.size() - (prefixLength + fileName.size()));

	try {
		WR3CK_ASSERT(m_fileDataMap.find(fileStoreName) == m_fileDataMap.end(), "File name \"%s\" hides an existing file.", fileStoreName.c_str());
		m_fileDataMap.emplace(fileStoreName, filePath);
	} catch (const std::exception& exception) {
		WR3CK_ERROR("Failed to load asset file \"%s\".\nReason: \"%s\"", filePath.generic_string().c_str(), exception.what());
	}

	const auto it = m_fileDataMap.find(fileStoreName);
	WR3CK_ASSERT(it != m_fileDataMap.end());

	float compressionRatio = static_cast<float>(it->second.dataSizeCompressed()) / static_cast<float>(it->second.dataSizeUncompressed());
	if (it->second.dataSizeUncompressed() <= 0)
		compressionRatio = 1.0f;
	WR3CK_LOG("> Added asset file \"%s\", compressed by %2.2f%%\n", it->first.c_str(), (1.0f - compressionRatio) * 100.0f);
}

void Packager::saveToFile(const std::string& path) {
	FILE* const file = fopen(path.c_str(), "wb");
	WR3CK_ASSERT(file != nullptr, "Failed to open package file \"%s\" for writing.", path.c_str());

	Internal::AssetPackage::Header header;
	header.m_signature = Internal::AssetPackage::Constants::fileSignature;
	header.m_fileCount = m_fileDataMap.size();
	header.m_infoStart = sizeof(header);
	fwrite(&header, sizeof(header), 1, file);

	const size_t totalInfoSize = sizeof(Internal::AssetPackage::Info) * header.m_fileCount;
	size_t totalNameSize = 0;
	for (const auto& it : m_fileDataMap) {
		totalNameSize += sizeof(char) * (it.first.size() + 1);
	}

	// Info.
	WR3CK_ASSERT(ftell(file) == sizeof(header));
	size_t nameOffset = sizeof(header) + totalInfoSize;
	size_t dataOffset = nameOffset + totalNameSize;
	for (const auto& it : m_fileDataMap) {
		Internal::AssetPackage::Info info;
		info.m_type = it.second.type();
		info.m_dataSizeCompressed = it.second.dataSizeCompressed();
		info.m_dataSizeUncompressed = it.second.dataSizeUncompressed();

		info.m_nameStart = nameOffset;
		nameOffset += sizeof(char) * (it.first.size() + 1);

		info.m_dataStart = dataOffset;
		dataOffset += info.m_dataSizeCompressed;

		fwrite(&info, sizeof(info), 1, file);
	}

	// Names.
	WR3CK_ASSERT(ftell(file) == sizeof(header) + totalInfoSize);
	for (const auto& it : m_fileDataMap) {
		fwrite(it.first.c_str(), sizeof(char), it.first.size() + 1, file);
	}

	// Data.
	WR3CK_ASSERT(ftell(file) == sizeof(header) + totalInfoSize + totalNameSize);
	for (const auto& it : m_fileDataMap) {
		fwrite(it.second.dataCompressed(), sizeof(uint8_t), it.second.dataSizeCompressed(), file);
	}

	fclose(file);
}
}