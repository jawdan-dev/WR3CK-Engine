#include <WR3CK/asset/renderTexture.hpp>

namespace WR3CK
{
RenderTextureData::RenderTextureData(const int width, const int height) :
	m_width(0), m_height(0),
	m_frameBuffer(0), m_textureId(0), m_renderBuffer(0), m_textureHandle(0) {
	resize(width, height);
}
RenderTextureData::~RenderTextureData() {
	WR3CK_CLEANUP_GL(m_frameBuffer, glDeleteFramebuffers(1, &m_frameBuffer););
	WR3CK_CLEANUP_GL(m_textureId, glDeleteTextures(1, &m_textureId); m_textureHandle = 0);
	WR3CK_CLEANUP_GL(m_renderBuffer, glDeleteRenderbuffers(1, &m_renderBuffer););
	m_width = 0;
	m_height = 0;
}

const GLuint RenderTextureData::frameBuffer() const {
	return m_frameBuffer;
}

void RenderTextureData::resize(const uint32_t _width, const uint32_t _height) {
	const uint32_t width = _height <= 0 ? 0 : _width,
				   height = _width <= 0 ? 0 : _height;
	if (width == m_width && height == m_height) {
		return;
	}

	if (width == 0 || height == 0) {
		RenderTextureData::~RenderTextureData();
		return;
	}

	m_width = width;
	m_height = height;

	// Setup texture.
	if (m_textureId == 0) {
		glGenTextures(1, &m_textureId);
	}
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_textureHandle = glGetTextureHandleARB(m_textureId);
	glMakeTextureHandleResidentARB(m_textureHandle);

	// Setup renderbuffer.
	if (m_renderBuffer == 0) {
		glGenRenderbuffers(1, &m_renderBuffer);
	}
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Setup framebuffer
	if (m_frameBuffer == 0) {
		glGenFramebuffers(1, &m_frameBuffer);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureId, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer, 0);
	WR3CK_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Failed to generate RenderTexture's framebuffer.");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
}