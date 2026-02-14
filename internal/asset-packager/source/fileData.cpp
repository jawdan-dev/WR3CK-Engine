#include "fileData.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <zlib.h>

namespace WR3CK
{
FileData::FileData(const std::filesystem::path& path) :
	m_dataUncompressed(nullptr), m_dataCompressed(nullptr),
	m_dataSizeUncompressed(SIZE_MAX), m_dataSizeCompressed(SIZE_MAX),
	m_type(Internal::AssetPackage::DataType::COUNT) {
	(this->*getExtensionFunction(path))(path.generic_string());
	WR3CK_ASSERT(m_dataUncompressed != nullptr, "Failed to load file data.");
	WR3CK_ASSERT(m_dataSizeUncompressed != SIZE_MAX, "Failed to load file data size.");
	WR3CK_ASSERT(m_type < Internal::AssetPackage::DataType::COUNT, "Failed to load file type information.");

	compressData();
	WR3CK_ASSERT(m_dataCompressed != nullptr && m_dataSizeCompressed != SIZE_MAX, "Failed to compress file data.");
}
FileData::~FileData() {
	WR3CK_CLEANUP(m_dataUncompressed, free(m_dataUncompressed); m_dataSizeUncompressed = SIZE_MAX);
	WR3CK_CLEANUP(m_dataCompressed, free(m_dataCompressed); m_dataSizeCompressed = SIZE_MAX);
	m_type = Internal::AssetPackage::DataType::COUNT;
}

const FileData::ExtensionFunc FileData::getExtensionFunction(const std::filesystem::path& path) const {
	static std::map<std::string, ExtensionFunc> s_functionMap;
	if (s_functionMap.empty()) {
		// Supported shader formats.
		s_functionMap.emplace(".glsl", &loadShader);

		// Supported texture formats.
		s_functionMap.emplace(".jpeg", &loadTexture);
		s_functionMap.emplace(".jpg", &loadTexture);
		s_functionMap.emplace(".bmp", &loadTexture);
		s_functionMap.emplace(".png", &loadTexture);
		s_functionMap.emplace(".gif", &loadTexture); //
		s_functionMap.emplace(".tga", &loadTexture); //
		s_functionMap.emplace(".psd", &loadTexture); //
		s_functionMap.emplace(".hdr", &loadTexture); //
		s_functionMap.emplace(".pnm", &loadTexture); //
	}

	std::string lcExtension = path.extension().generic_string();
	for (char& c : lcExtension) {
		c = std::tolower(c);
	}

	const auto it = s_functionMap.find(lcExtension);
	if (it == s_functionMap.end())
		return &loadPlaintext;

	return it->second;
}
void FileData::compressData() {
	WR3CK_ASSERT(m_dataUncompressed != nullptr);
	WR3CK_ASSERT(m_dataSizeUncompressed != SIZE_MAX);

	uLongf sourceSize = m_dataSizeUncompressed;
	Bytef* source = reinterpret_cast<Bytef*>(m_dataUncompressed);
	WR3CK_ASSERT(sourceSize == m_dataSizeUncompressed);
	uLongf destinationSize = sourceSize;
	Bytef* destination = reinterpret_cast<Bytef*>(malloc(destinationSize));
	const int compressStatus = compress(destination, &destinationSize, source, sourceSize);

	m_dataCompressed = destination;
	if (destinationSize < sourceSize) {
		WR3CK_ASSERT(compressStatus == 0);
		m_dataSizeCompressed = destinationSize;
	} else {
		memcpy(destination, source, sourceSize);
		m_dataSizeCompressed = sourceSize;
	}
}


void FileData::loadPlaintext(const std::string& path) {
	FILE* file = fopen(path.c_str(), "rb");
	WR3CK_ASSERT(file != nullptr, "Failed to open file \"%s\".", path.c_str());

	fseek(file, 0, SEEK_END);
	const size_t fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	const size_t storageSize = fileSize + 1;
	Internal::AssetPackage::PlaintextData* data = reinterpret_cast<Internal::AssetPackage::PlaintextData*>(malloc(storageSize));
	fread(&data->m_data, sizeof(char), fileSize, file);
	(&data->m_data)[fileSize] = '\0';

	m_dataUncompressed = data;
	m_dataSizeUncompressed = storageSize;
	m_type = Internal::AssetPackage::DataType::Plaintext;
}
void FileData::loadShader(const std::string& path) {
	loadPlaintext(path);
	m_type = Internal::AssetPackage::DataType::Shader;
}
void FileData::loadTexture(const std::string& path) {
	int width, height, channels;
	stbi_uc* textureData = stbi_load(path.c_str(), &width, &height, &channels, 0);
	WR3CK_ASSERT(textureData != nullptr, "Failed to open and interpret texture file \"%s\".", path.c_str());

	const size_t textureSize = width * height * channels;
	const size_t dataSize = (sizeof(Internal::AssetPackage::TextureData) - sizeof(uint8_t)) + textureSize;

	Internal::AssetPackage::TextureData* data = reinterpret_cast<Internal::AssetPackage::TextureData*>(malloc(dataSize));
	data->m_width = width;
	data->m_height = height;
	data->m_channels = channels;
	memcpy(&data->m_data, textureData, textureSize);
	stbi_image_free(textureData);

	m_dataUncompressed = data;
	m_dataSizeUncompressed = dataSize;
	m_type = Internal::AssetPackage::DataType::Texture;
}
}
