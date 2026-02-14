#include <WR3CK/file/assetLoader.hpp>

#include <zlib.h>

extern char _binary_assets_pck_start;
extern char _binary_assets_pck_end;

namespace WR3CK
{
AssetLoader AssetLoader::s_instance;

#define AssetLoaderFunction(name, ...)                                                                                    \
	const name##Handle AssetLoader::get##name(const std::string& path) {                                                  \
		auto it = s_instance.m_loaded##name.find(path);                                                                   \
		if (it != s_instance.m_loaded##name.end()) {                                                                      \
			if (it->second.hasData())                                                                                     \
				return name##Handle(it->second, false);                                                                   \
			s_instance.m_loaded##name.erase(it);                                                                          \
		}                                                                                                                 \
                                                                                                                          \
		void* const rawData = s_instance.loadFile(path, Internal::AssetPackage::DataType::name);                          \
		const Internal::AssetPackage::name##Data& data = *reinterpret_cast<Internal::AssetPackage::name##Data*>(rawData); \
                                                                                                                          \
		name##Handle handle(__VA_ARGS__);                                                                                 \
		it = s_instance.m_loaded##name.emplace(path, handle).first;                                                       \
		it->second.makeWeak();                                                                                            \
                                                                                                                          \
		free(rawData);                                                                                                    \
		return name##Handle(handle);                                                                                      \
	}
AssetLoaderFunction(Plaintext, &data.m_data);
AssetLoaderFunction(Shader, &data.m_data);
AssetLoaderFunction(Texture, data.m_width, data.m_height, &data.m_data, data.m_channels);

AssetLoader::AssetLoader() :
	m_fileInformation(),
	m_loadedPlaintext(), m_loadedTexture(), m_loadedShader() {
	const void* fileStart = reinterpret_cast<void*>(&_binary_assets_pck_start);
	const Internal::AssetPackage::Header& header = *reinterpret_cast<const Internal::AssetPackage::Header*>(fileStart);
	WR3CK_ASSERT(header.m_signature == Internal::AssetPackage::Constants::fileSignature, "Failed to match asset package signature.");

	const void* file = fileStart + header.m_infoStart;
	for (uint32_t i = 0; i < header.m_fileCount; i++) {
		const Internal::AssetPackage::Info& info = *reinterpret_cast<const Internal::AssetPackage::Info*>(file);
		file += sizeof(info);

		const char* const name = reinterpret_cast<const char*>(fileStart + info.m_nameStart);
		FileInformation fileInformation;
		fileInformation.m_info = &info;
		fileInformation.m_data = fileStart + info.m_dataStart;
		m_fileInformation.emplace(name, fileInformation);
	}
}

void* const AssetLoader::loadFile(const std::string& path, Internal::AssetPackage::DataType type) const {
	const auto it = s_instance.m_fileInformation.find(path);
	WR3CK_ASSERT(it != s_instance.m_fileInformation.end(), "Asset file \"%s\n could not be found in the asset package.", path.c_str());
	WR3CK_ASSERT(it->second.m_info->m_type == type, "Asset file \"%s\n is not the expected file type.", path.c_str());

	const uLongf sourceSize = it->second.m_info->m_dataSizeCompressed;
	const Bytef* const source = reinterpret_cast<const Bytef* const>(it->second.m_data);
	uLongf destinationSize = it->second.m_info->m_dataSizeUncompressed;
	Bytef* const destination = reinterpret_cast<Bytef* const>(malloc(destinationSize));

	if (sourceSize < destinationSize) {
		const int uncompressStatus = uncompress(destination, &destinationSize, source, sourceSize);
		WR3CK_ASSERT(uncompressStatus == 0);
		WR3CK_ASSERT(destinationSize == it->second.m_info->m_dataSizeUncompressed);
	} else {
		memcpy(destination, source, sourceSize);
	}

	return destination;
}
}