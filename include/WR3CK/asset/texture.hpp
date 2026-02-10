#pragma once
#include <WR3CK/asset/assetHandle.hpp>
#include <WR3CK/core/opengl.hpp>

namespace WR3CK
{
class TextureData : public AssetBaseData {
public:
	TextureData(const uint32_t width, const uint32_t height);
	TextureData(const uint32_t width, const uint32_t height, const uint8_t* const data, const uint8_t channels = 4);
	TextureData(const TextureData& other) = delete;
	~TextureData();

	WR3CK_GETTER_CONST(Color* const, data, m_data);
	WR3CK_GETTER_CONST(uint32_t, width, m_width);
	WR3CK_GETTER_CONST(uint32_t, height, m_height);
	WR3CK_GETTER_CONST(GLuint, texture, m_textureId);
	WR3CK_GETTER_CONST(GLuint64, handle, m_textureHandle);

	const Color& getPixel(const uint32_t x, const uint32_t y) const;

	void setPixel(const uint32_t x, const uint32_t y, const Color& color);
	void resize(const uint32_t width, const uint32_t height);

	void commit();

private:
	Color* m_data;
	uint32_t m_width, m_height;
	GLuint m_textureId;
	GLuint64 m_textureHandle;
	bool m_modified;
};
typedef AssetHandle<TextureData> TextureHandle;
}