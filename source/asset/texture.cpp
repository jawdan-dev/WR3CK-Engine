#include <WR3CK/asset/texture.hpp>

namespace WR3CK
{
TextureData::TextureData(const uint32_t width, const uint32_t height) :
	m_data(nullptr),
	m_width(0), m_height(0),
	m_textureId(0), m_textureHandle(0),
	m_modified(false) {
	resize(width, height);
}
TextureData::TextureData(const uint32_t width, const uint32_t height, const uint8_t* const data, const uint8_t channels) :
	TextureData(width, height) {
	WR3CK_ASSERT(data != nullptr);
	for (size_t y = 0; y < m_height; y++) {
		for (size_t x = 0; x < m_width; x++) {
			const uint8_t* const pixel = &data[(x + (y * m_width)) * channels];
			switch (channels) {
				default: WR3CK_ASSERT(false); break;
				case 1: m_data[x + (y * m_width)] = Color(pixel[0], pixel[0], pixel[0]); break;
				case 2: m_data[x + (y * m_width)] = Color(pixel[0], pixel[0], pixel[0], pixel[1]); break;
				case 3: m_data[x + (y * m_width)] = Color(pixel[0], pixel[1], pixel[2]); break;
				case 4: m_data[x + (y * m_width)] = Color(pixel[0], pixel[1], pixel[2], pixel[3]); break;
			}
		}
	}
	m_modified = m_modified || (m_width > 0 && m_height > 0);
	commit();
}
TextureData::~TextureData() {
	WR3CK_CLEANUP(m_data, delete[] m_data);
	WR3CK_CLEANUP_GL(m_textureId, glDeleteTextures(1, &m_textureId); m_textureHandle = 0);
	m_width = 0;
	m_height = 0;
	m_modified = false;
}

const Color& TextureData::getPixel(const uint32_t x, const uint32_t y) const {
	WR3CK_ASSERT(x < m_width, "X outside of texture dimensions.");
	WR3CK_ASSERT(y < m_height, "Y outside of texture dimensions.");
	return m_data[x + (y * m_width)];
}

void TextureData::setPixel(const uint32_t x, const uint32_t y, const Color& color) {
	WR3CK_ASSERT(x < m_width, "X outside of texture dimensions.");
	WR3CK_ASSERT(y < m_height, "Y outside of texture dimensions.");
	m_data[x + (y * m_width)] = color;
	m_modified = true;
}
void TextureData::resize(const uint32_t _width, const uint32_t _height) {
	const uint32_t width = _height <= 0 ? 0 : _width,
				   height = _width <= 0 ? 0 : _height;
	if (width == m_width && height == m_height) {
		return;
	}

	if (width == 0 || height == 0) {
		TextureData::~TextureData();
		return;
	}

	const uint32_t compatibleWidth = __min(width, m_width),
				   compatibleHeight = __min(height, m_height);

	Color* const newData = new Color[width * height](0);
	for (uint32_t y = 0; y < compatibleHeight; y++) {
		for (uint32_t x = 0; x < compatibleWidth; x++) {
			newData[x + (y * width)] = m_data[x + (y * m_width)];
		}
	}
	WR3CK_CLEANUP(m_data, delete[] m_data);

	m_data = newData;
	m_width = width;
	m_height = height;

	m_modified = true;
}

void TextureData::commit() {
	if (!m_modified)
		return;
	m_modified = false;

	if (m_data == nullptr) {
		WR3CK_CLEANUP_GL(m_textureId, glDeleteTextures(1, &m_textureId); m_textureHandle = 0);
		return;
	}

	if (m_textureId == 0) {
		glGenTextures(1, &m_textureId);
	}
	// TODO: Subpixel update.
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_FLOAT, m_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_textureHandle = glGetTextureHandleARB(m_textureId);
	glMakeTextureHandleResidentARB(m_textureHandle);
}
}