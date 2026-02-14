#pragma once
#include <WR3CK/core/core.hpp>

/**
 * Package Structure:
 * ----------------------------------- *
 *             File Header             *
 * ----------------------------------- *
 *             File Info[0]            *
 *                 ...                 *
 *             File Info[n]            *
 * ----------------------------------- *
 *         File Name[0] (c_str)        *
 *                 ...                 *
 *         File Name[n] (c_str)        *
 * ----------------------------------- *
 *             File Data[0]            *
 *                 ...                 *
 *             File Data[n]            *
 * ----------------------------------- *
 */

namespace WR3CK::Internal::AssetPackage
{
enum class DataType : uint8_t {
	Plaintext,
	Shader,
	Texture,
	COUNT
};

typedef uint64_t signature_t;

namespace Constants
{
constexpr signature_t fileSignature = 0x4B43506968737553;
}

#pragma pack(push, 1)
struct Header {
	signature_t m_signature;
	uint32_t m_fileCount;
	size_t m_infoStart;
};
struct Info {
	size_t m_nameStart;
	DataType m_type;
	size_t m_dataStart;
	uint32_t m_dataSizeCompressed, m_dataSizeUncompressed;
};

struct PlaintextData {
	char m_data;
};
typedef PlaintextData ShaderData;
struct TextureData {
	uint32_t m_width, m_height;
	uint32_t m_channels;
	uint8_t m_data;
};
#pragma pack(pop)
}