#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/file/assetPackage.hpp>

#include <filesystem>

namespace WR3CK
{
class FileData {
public:
	FileData(const std::filesystem::path& path);
	FileData(const FileData& other) = delete;
	~FileData();

	WR3CK_GETTER_CONST(void* const, dataUncompressed, m_dataUncompressed);
	WR3CK_GETTER_CONST(void* const, dataCompressed, m_dataCompressed);
	WR3CK_GETTER_CONST(size_t, dataSizeUncompressed, m_dataSizeUncompressed);
	WR3CK_GETTER_CONST(size_t, dataSizeCompressed, m_dataSizeCompressed);
	WR3CK_GETTER_CONST(Internal::AssetPackage::DataType, type, m_type);

private:
	typedef void (FileData::*ExtensionFunc)(const std::string&);
	const ExtensionFunc getExtensionFunction(const std::filesystem::path& path) const;
	void compressData();

	void loadPlaintext(const std::string& path);
	void loadShader(const std::string& path);
	void loadTexture(const std::string& path);

	void *m_dataUncompressed, *m_dataCompressed;
	size_t m_dataSizeUncompressed, m_dataSizeCompressed;
	Internal::AssetPackage::DataType m_type;
};
}