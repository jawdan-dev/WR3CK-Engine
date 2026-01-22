#pragma once
#include <WR3CK/asset/frameBuffer.hpp>

namespace WR3CK
{
class RenderTextureData : public FrameBufferData {
public:
	RenderTextureData(const int width, const int height);
	RenderTextureData(const RenderTextureData& other) = delete;
	virtual ~RenderTextureData();

	WR3CK_GETTER_CONST(int, width, m_width);
	WR3CK_GETTER_CONST(int, height, m_height);
	virtual const GLuint frameBuffer() const override;
	WR3CK_GETTER_CONST(GLuint, texture, m_textureId);
	WR3CK_GETTER_CONST(GLuint, renderBuffer, m_renderBuffer);
	WR3CK_GETTER_CONST(GLuint64, handle, m_textureHandle);

	void resize(const uint32_t width, const uint32_t height);

private:
	uint32_t m_width, m_height;
	GLuint m_frameBuffer, m_textureId, m_renderBuffer;
	GLuint64 m_textureHandle;
};
typedef AssetHandle<RenderTextureData> RenderTextureHandle;
}