#pragma once
#include <WR3CK/asset/assetHandle.hpp>

namespace WR3CK
{
class TextureData : public AssetBaseData {
public:
	TextureData(const uint32_t width, const uint32_t height);
	TextureData(const TextureData& other) = delete;
	~TextureData();

	WR3CK_GETTER_CONST(uint32_t*, data, m_data);
	WR3CK_GETTER_CONST(uint32_t, width, m_width);
	WR3CK_GETTER_CONST(uint32_t, height, m_height);
	WR3CK_GETTER_CONST(GLuint, texture, m_textureId);
	WR3CK_GETTER_CONST(GLuint64, handle, m_textureHandle);

	const uint32_t getPixel(const uint32_t x, const uint32_t y) const;

	void setPixel(const uint32_t x, const uint32_t y, const uint32_t color);
	void resize(const uint32_t width, const uint32_t height);

	void commit();

private:
	// TODO: Change data to use  Color class type?
	uint32_t* m_data;
	uint32_t m_width, m_height;
	GLuint64 m_textureHandle; //
	GLuint m_textureId;
	bool m_modified;
};
typedef AssetHandle<TextureData> TextureHandle;
}